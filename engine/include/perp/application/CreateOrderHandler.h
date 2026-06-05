
#pragma once

#include <string>
#include <inttypes.h>

#include "../domain/RiskEngine.h";
#include "../domain/MatchingEngine.h"
#include "../domain/PositionManager.h"
#include "../domain/Account.h"
#include "../domain/types.h"
#include "../status/status.h"
#include "../util/macros.h"
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

    class CreateOrderResponse
    {
    };

    class CreateOrderHandler
    {

        DOMAIN::RiskEngine &riskEngine;
        DOMAIN::MatchingEngine &matchingEngine;
        DOMAIN::PositionManager &positionManager;
        DOMAIN::Account &account;

        CreateOrderHandler(DOMAIN::RiskEngine &riskEngine_, DOMAIN::MatchingEngine &matchingEngine_, DOMAIN::PositionManager &positionManager_, DOMAIN::Account &account_)
            : riskEngine(riskEngine_), matchingEngine(matchingEngine_), positionManager(positionManager_), account(account_) {}

        static STATUS::StatusOr<DOMAIN::Order> commandToDomain(CreateOrderCommand command) {}

        STATUS::StatusOr<CreateOrderResponse> handle(CreateOrderCommand command)
        {
            ASSIGN_OR_RETURN(order, commandToDomain(command));

            ASSIGN_OR_RETURN(marginRequired, riskEngine.evaluateOrder(order));

            ASSIGN_OR_RETURN(updatedBal, account.lockBalance(order.userId, order.margin));

            // ASSIGN_OR_RETURN(([ trades, returnedMargins ]), matchingEngine.placeOrder(order));

            // ASSIGN_OR_RETURN(userPnl, positionManager.applyTrades(trades));
        }
    };

}