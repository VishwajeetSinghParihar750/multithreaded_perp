
#pragma once

#include <string>
#include <inttypes.h>

#include "eventBus.h"

#include "../domain/riskEngine.h";
#include "../domain/matchingEngine.h"
#include "../domain/positionManager.h"
#include "../domain/account.h"
#include "../status/status.h"

#include "../domain/types.h"
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

        EventBus &eventBus;

        DOMAIN::RiskEngine &riskEngine;
        DOMAIN::MatchingEngine &matchingEngine;
        DOMAIN::PositionManager &positionManager;
        DOMAIN::Account &account;

    public:
        CreateOrderHandler(EventBus &eventBus_, DOMAIN::RiskEngine &riskEngine_, DOMAIN::MatchingEngine &matchingEngine_, DOMAIN::PositionManager &positionManager_, DOMAIN::Account &account_)
            : eventBus(eventBus_), riskEngine(riskEngine_), matchingEngine(matchingEngine_), positionManager(positionManager_), account(account_) {}

        DOMAIN::Order commandToDomain(CreateOrderCommand command)
        {
        }

        STATUS::StatusOr<CreateOrderResponse> handle(CreateOrderCommand command)
        {
            auto order = commandToDomain(command);
            ASSIGN_OR_RETURN(marginRequired, riskEngine.evaluateOrder(order));
            ASSIGN_OR_RETURN(updatedBal, account.lockBalance(order.userId, order.margin));

            auto events = matchingEngine.placeOrder(order);

            for (const auto &ev : events)
                eventBus.emit(ev);
        }
    };

}