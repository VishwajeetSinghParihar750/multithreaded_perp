#pragma once
#include <string>

#include "../application/eventBus.h"
#include "types.h"
#include "../event/event.h"

namespace DOMAIN
{
    class Market
    {
        MARKET_ID marketId;
        PRICE indexPrice; // real exchange price
        PRICE markPrice;  // current exchange price

        void handleTradePriceUpdates(const TradesCreated &event)
        {
            // set markprice to last traded price
            if (event.trades.empty())
                return;
            markPrice = event.trades.back().price;
        }

    public:
        Market(MARKET_ID marketId_, PRICE indexPrice_, EventBus &eventBus) : marketId(marketId_), indexPrice(indexPrice_), markPrice(0)
        {
            eventBus.subscribe<TradesCreated>([this](const TradesCreated &event) -> void
                                              { this->handleTradePriceUpdates(event); });
        }

        PRICE getIndexPrice() const
        {
            return indexPrice;
        }
        PRICE getMarkPrice() const
        {
            return markPrice;
        }
        void setIndexPrice(PRICE indexPrice_)
        {
            indexPrice = indexPrice_;
        }
    };
}