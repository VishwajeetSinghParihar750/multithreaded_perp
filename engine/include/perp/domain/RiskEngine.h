#pragma once

#include "Account.h"
#include "expected"
#include "inttypes.h"
#include "Order.h"

enum class RISK_ERROR
{
    INSUFFICIENT_BALANCE
};

class RiskEngine
{

    Account &account;

    RiskEngine(Account account_) : account(account_) {}

    // return margin required for this order
    std::expected<uint64_t, RISK_ERROR> evaluateOrder(Order order)
    {
        //
    }
};