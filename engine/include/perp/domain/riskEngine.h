#pragma once

#include <memory>

#include "account.h"
#include "expected"
#include "inttypes.h"
#include "order.h"
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

    public:
        RiskEngine(Account account_) : account(account_) {}

        // return margin required for this order
        STATUS::StatusOr<PRICE> evaluateOrder(const std::unique_ptr<Order> &order)
        {
        }
        // return margin required from both orders
        std::tuple<PRICE, PRICE> evaluateTrade(const std::unique_ptr<Order> &order1, const std::unique_ptr<Order> &order2)
        {
        }
    };
} // namespace DOMAIN