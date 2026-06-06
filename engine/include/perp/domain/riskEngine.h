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
        STATUS::StatusOr<uint64_t> evaluateOrder(const std::unique_ptr<Order> &order)
        {
        }
    };
} // namespace DOMAIN