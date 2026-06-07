
#pragma once

#include <string>
#include <inttypes.h>
#include <memory>

#include "eventBus.h"
#include "../domain/riskEngine.h";
#include "../domain/orderbook.h"
#include "../domain/idProvider.h"
#include "../domain/positionManager.h"
#include "../domain/account.h"
#include "../status/status.h"

#include "../domain/types.h"
#include "../util/macros.h"
namespace APPLICATION
{

    class CreateOrderCommand
    {
    public:
        DOMAIN::USER_ID userId;
        DOMAIN::PRICE price;
        DOMAIN::QUANTITY quantity;
        DOMAIN::PRICE margin;
        DOMAIN::MARKET_ID marketId;
        DOMAIN::SIDE side;
        DOMAIN::ORDER_TYPE type;
        DOMAIN::MARGIN_TYPE marginType;
    };

    class CreateOrderHandler
    {

        DOMAIN::RiskEngine &riskEngine;
        DOMAIN::Orderbook &orderbook;
        DOMAIN::Account &account;
        DOMAIN::IdProvider &idProvider;

        std::unique_ptr<DOMAIN::Order> commandToOrder(const CreateOrderCommand &command)
        {
            return std::make_unique<DOMAIN::Order>(
                DOMAIN::Order{
                    command.userId, command.price, command.quantity, command.margin,
                    0, command.marketId, DOMAIN::ORDER_STATUS::OPEN, command.side, command.type,
                    command.marginType, idProvider.getNextOrderId()});
        }

    public:
        CreateOrderHandler(EventBus &eventBus_, DOMAIN::IdProvider &idProvider_, DOMAIN::RiskEngine &riskEngine_,
                           DOMAIN::Orderbook &orderbook_, DOMAIN::Account &account_)
            : riskEngine(riskEngine_), orderbook(orderbook_), account(account_), idProvider(idProvider_) {}

        STATUS::StatusOr<void> handle(CreateOrderCommand command)
        {
            auto order = commandToOrder(command);

            // check preconditions
            ASSIGN_OR_RETURN(marginRequired, riskEngine.evaluateOrder(order));
            ASSIGN_OR_RETURN(updatedBal, account.lockBalance(order->userId, order->margin));

            // now everything will be taken care of with events, order cant be rejected after this
            // only events can be reacted to
            orderbook.placeOrder(std::move(order));
        }
    };
}