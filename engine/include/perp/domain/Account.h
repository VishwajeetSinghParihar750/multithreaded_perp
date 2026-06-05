
#pragma once
#include "types.h"
#include <unordered_map>
#include <unordered_set>
#include "inttypes.h"
#include <expected>
#include <assert.h>
namespace DOMAIN
{

    enum class ACCOUNT_ERROR
    {
        USER_DOES_NOT_EXIST,
        EXCEEDING_MAX_BALANCE_LIMIT,
        INSUFFICIENT_BALANCE,
    };

    std::string_view accountErrorToStringView(const ACCOUNT_ERROR &error)
    {
        switch (error)
        {
        case ACCOUNT_ERROR::EXCEEDING_MAX_BALANCE_LIMIT:
            return "EXCEEDING_MAX_BALANCE_LIMIT";
        case ACCOUNT_ERROR::INSUFFICIENT_BALANCE:
            return "INSUFFICIENT_BALANCE";
        case ACCOUNT_ERROR::USER_DOES_NOT_EXIST:
            return "USER_DOES_NOT_EXIST";
        default:
            return "";
        }
    }

    class Balance
    {
    public:
        uint64_t balance = 0;
        uint64_t lockedBalance = 0;
    };

    // this nees to be thread safe
    class Account
    {

        std::unordered_map<USER_ID, Balance> balance;

        std::expected<Balance, ACCOUNT_ERROR> getBalance(USER_ID userId) const
        {
            if (!balance.contains(userId))
                return std::unexpected(ACCOUNT_ERROR::USER_DOES_NOT_EXIST);

            auto it = balance.find(userId);

            assert(it != balance.end());

            return it->second;
        }
        // return new balance
        std::expected<Balance, ACCOUNT_ERROR> addBalance(USER_ID userId, uint64_t amount)
        {
            // it should not excedd uin64_t limit

            uint64_t canAdd = UINT64_MAX - balance[userId].balance;

            if (canAdd < amount)
                return std::unexpected(ACCOUNT_ERROR::EXCEEDING_MAX_BALANCE_LIMIT);

            balance[userId].balance += amount;

            return balance[userId];
        }
        // return new balance
        std::expected<Balance, ACCOUNT_ERROR> removeBalance(USER_ID userId, uint64_t amount)
        {
            if (balance[userId].balance < amount)
                return std::unexpected(ACCOUNT_ERROR::INSUFFICIENT_BALANCE);

            balance[userId].balance -= amount;

            return balance[userId];
        }

        std::expected<Balance, ACCOUNT_ERROR> lockBalance(USER_ID userId, uint64_t amount)
        {
            balance[userId]; // will create if not already

            if (balance[userId].balance < amount)
                return std::unexpected(ACCOUNT_ERROR::INSUFFICIENT_BALANCE);

            balance[userId].balance -= amount;
            balance[userId].lockedBalance += amount;

            return balance[userId];
        }
    };

} // namespace DOMAIN