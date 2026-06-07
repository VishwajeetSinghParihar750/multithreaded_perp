#pragma once

#include <unordered_map>

#include "types.h"

namespace DOMAIN
{

    // this to be created per symbol
    class IdProvider
    {

        using IdCounter = uint64_t;
        MARKET_ID symbol;
        IdCounter perSymbolTradeId = 0;
        IdCounter perSymbolOrderId = 0;

    public:
        IdProvider(MARKET_ID symbol_) : symbol(symbol_) {}

        TRADE_ID getNextTradeId()
        {
            return symbol.append(std::to_string(perSymbolTradeId++));
        }

        ORDER_ID getNextOrderId()
        {
            return symbol.append(std::to_string(perSymbolOrderId++));
        }
    };

}