
#pragma once
#include "types.h"
#include <unordered_map>
#include <unordered_set>
#include "inttypes.h"
#include <expected>
#include <assert.h>
#include "../status/status.h"
#include "../event/event.h"
#include "eventBus.h"
namespace DOMAIN
{

    // this nees to be thread safe
    class Account
    {
        class Balance
        {
        public:
            uint64_t balance = 0;
            uint64_t lockedBalance = 0;
        };
        enum class ACCOUNT_ERROR
        {
            USER_DOES_NOT_EXIST,
            EXCEEDING_MAX_BALANCE_LIMIT,
            INSUFFICIENT_BALANCE,
        };

        static constexpr std::string_view accountErrorToStringView(const ACCOUNT_ERROR &error)
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

        //
        void handleUserPnl(const userPnlCreated &userPnl)
        {
            balance[userPnl.userId].lockedBalance -= userPnl.releasedMargin;
            balance[userPnl.userId].balance += userPnl.pnl;

            assert(balance[userPnl.userId].lockedBalance >= 0);
            assert(balance[userPnl.userId].balance >= 0);
        }

        // data
        std::unordered_map<USER_ID, Balance> balance;

    public:
        Account(EventBus &eventBus)
        {

            eventBus.subscribe<userPnlCreated>([this](const userPnlCreated &userPnl)
                                               { this->handleUserPnl(userPnl); });
        }

        STATUS::StatusOr<Balance> getBalance(USER_ID userId)
        {
            return balance[userId];
        }
        // return new balance
        STATUS::StatusOr<Balance> addBalance(USER_ID userId, uint64_t amount)
        {
            // it should not excedd uin64_t limit

            uint64_t canAdd = UINT64_MAX - balance[userId].balance;

            if (canAdd < amount)
                return std::unexpected(STATUS::Status(STATUS::StatusCode::kFailedPrecondition,
                                                      accountErrorToStringView(ACCOUNT_ERROR::EXCEEDING_MAX_BALANCE_LIMIT)));

            balance[userId].balance += amount;

            return balance[userId];
        }

        // return new balance
        STATUS::StatusOr<Balance> removeBalance(USER_ID userId, uint64_t amount)
        {
            if (balance[userId].balance < amount)
                return std::unexpected(STATUS::Status(STATUS::StatusCode::kFailedPrecondition,
                                                      accountErrorToStringView(ACCOUNT_ERROR::INSUFFICIENT_BALANCE)));

            balance[userId].balance -= amount;

            return balance[userId];
        }

        STATUS::StatusOr<Balance> lockBalance(USER_ID userId, uint64_t amount)
        {
            balance[userId]; // will create if not already

            if (balance[userId].balance < amount)
                return std::unexpected(STATUS::Status(STATUS::StatusCode::kFailedPrecondition,
                                                      accountErrorToStringView(ACCOUNT_ERROR::INSUFFICIENT_BALANCE)));

            balance[userId].balance -= amount;
            balance[userId].lockedBalance += amount;

            return balance[userId];
        }

        void unlockBalance(USER_ID userId, uint64_t amount)
        {
            assert(balance.contains(userId));
            assert(balance[userId].lockedBalance >= amount);

            balance[userId].balance += amount;
            balance[userId].lockedBalance -= amount;
        }
    };

} // namespace DOMAIN