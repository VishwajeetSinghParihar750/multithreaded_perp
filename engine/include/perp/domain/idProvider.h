#pragma once

#include <unordered_map>

#include "types.h"

namespace DOMAIN
{

    // this to be created per marketId
    class IdProvider
    {

        using IdCounter = uint64_t;
        MARKET_ID marketId;
        IdCounter permarketIdTradeId = 0;
        IdCounter permarketIdOrderId = 0;

    public:
        IdProvider(MARKET_ID marketId_) : marketId(marketId_) {}

        TRADE_ID getNextTradeId()
        {
            return marketId.append(std::to_string(permarketIdTradeId++));
        }

        ORDER_ID getNextOrderId()
        {
            return marketId.append(std::to_string(permarketIdOrderId++));
        }
    };

}