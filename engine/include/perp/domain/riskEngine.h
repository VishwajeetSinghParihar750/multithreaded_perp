#pragma once

#include <memory>
#include "expected"

#include "account.h"
#include "inttypes.h"
#include "order.h"
#include "market.h"
#include "position.h"

#include "../status/status.h"

namespace DOMAIN
{

    enum class RISK_ERROR
    {
        INSUFFICIENT_BALANCE
    };

    class RiskEngine
    {

        Account &account;
        Market &market;

    public:
        RiskEngine(Account &account_, Market &market_) : account(account_), market(market_) {}
        // return margin required for this order
        STATUS::StatusOr<PRICE> evaluateOrder(const std::unique_ptr<Order> &order)
        {
        }
        // return margin required from both orders
        std::tuple<PRICE, PRICE> evaluateTrade(const std::unique_ptr<Order> &order1, const std::unique_ptr<Order> &order2)
        {
        }

        // get liquidation price
        PRICE getLiquidationPrice(const Position &position)
        {
            //
        }
    };
} // namespace DOMAIN