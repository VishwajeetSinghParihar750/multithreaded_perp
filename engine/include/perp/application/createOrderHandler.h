
#pragma once

#include <string>
#include <inttypes.h>
#include <memory>

#include "eventBus.h"
#include "../domain/riskEngine.h";
#include "../domain/matchingEngine.h"
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
        DOMAIN::MARKET_ID symbol;
        DOMAIN::SIDE side;
        DOMAIN::ORDER_TYPE type;
        DOMAIN::MARGIN_TYPE marginType;
    };

    class CreateOrderHandler
    {

        EventBus &eventBus;

        DOMAIN::RiskEngine &riskEngine;
        DOMAIN::MatchingEngine &matchingEngine;
        DOMAIN::Account &account;
        DOMAIN::IdProvider &idProvider;

        std::unique_ptr<DOMAIN::Order> commandToOrder(const CreateOrderCommand &command)
        {
            return std::make_unique<DOMAIN::Order>(
                DOMAIN::Order{
                    command.userId, command.price, command.quantity, command.margin,
                    0, command.symbol, DOMAIN::ORDER_STATUS::OPEN, command.side, command.type,
                    command.marginType, idProvider.getNextOrderId()});
        }

    public:
        CreateOrderHandler(EventBus &eventBus_, DOMAIN::IdProvider &idProvider_, DOMAIN::RiskEngine &riskEngine_, DOMAIN::MatchingEngine &matchingEngine_, DOMAIN::Account &account_)
            : eventBus(eventBus_), riskEngine(riskEngine_), matchingEngine(matchingEngine_), account(account_), idProvider(idProvider_) {}

        STATUS::StatusOr<void> handle(CreateOrderCommand command)
        {
            auto order = commandToOrder(command);
            ASSIGN_OR_RETURN(marginRequired, riskEngine.evaluateOrder(order));
            ASSIGN_OR_RETURN(updatedBal, account.lockBalance(order->userId, order->margin));

            auto events = matchingEngine.placeOrder(std::move(order));

            for (const auto &ev : events)
                eventBus.emit(ev);
        }
    };
}