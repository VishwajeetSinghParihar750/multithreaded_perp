
#pragma once

#include <string>
#include <inttypes.h>

#include "../domain/RiskEngine.h";
#include "../domain/Orderbook.h"
#include "../domain/PositionManager.h"
#include "../domain/Account.h"
#include "../domain/types.h"

namespace APPLICATION
{

    class CreateOrderCommand
    {
        DOMAIN::USER_ID userId;
        uint64_t price;
        uint64_t quantity;
        uint64_t margin;
        DOMAIN::TRADABLE_CURRENCY_SYMBOL symbol;
        DOMAIN::SIDE side;
        DOMAIN::ORDER_TYPE type;
        DOMAIN::MARGIN_TYPE marginType;
    };

    class CreateOrderError
    {
    };
    class CreateOrderResponse
    {
    };

    class CreateOrderHandler
    {

        RiskEngine &riskEngine;
        Orderbook &orderbook;
        PositionManager &positionManager;
        Account &account;

        CreateOrderHandler(RiskEngine &riskEngine_, Orderbook &orderbook_, PositionManager &positionManager_, Account &account_)
            : riskEngine(riskEngine_), orderbook(orderbook_), positionManager(positionManager_), account(account_) {}

        std::expected<CreateOrderResponse, CreateOrderError> handle(CreateOrderCommand command)
        {
        }
    };

}