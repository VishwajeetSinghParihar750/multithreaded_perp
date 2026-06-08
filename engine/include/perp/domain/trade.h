#pragma once

#include "types.h"
#include <memory>
#include <string>

namespace DOMAIN
{

    struct TradeOrderInfo
    {
        USER_ID userId;
        ORDER_ID orderId;
        QUANTITY filledQuantity;
        QUANTITY totalQuantity;
        ORDER_STATUS status;
        PRICE margin;
        MARGIN_TYPE marginType;
    };
    struct Trade
    {

        TRADE_ID tradeId;
        PRICE price;
        QUANTITY filledQuantity;
        MARKET_ID marketId;
        TradeOrderInfo longOrderInfo;
        TradeOrderInfo shortOrderInfo;
    };

    class TradeFactory
    {
        using IdCounter = uint64_t;
        MARKET_ID marketId;
        IdCounter tradeIdCounter = 0;

        TRADE_ID getNextTradeId()
        {
            return marketId.append(std::to_string(tradeIdCounter++));
        }

    public:
        TradeFactory(MARKET_ID marketId_) : marketId(marketId_) {}

        std::unique_ptr<Trade> createUniquePtr(PRICE price, QUANTITY filledQty, MARKET_ID marketId,
                                               TradeOrderInfo longOrderInfo, TradeOrderInfo shortOrderInfo)
        {
            return std::make_unique<Trade>(Trade{getNextTradeId(), price, filledQty, marketId, longOrderInfo, shortOrderInfo});
        }

        Trade create(PRICE price, QUANTITY filledQty, MARKET_ID marketId, TradeOrderInfo longOrderInfo, TradeOrderInfo shortOrderInfo)
        {
            return Trade{getNextTradeId(), price, filledQty, marketId, longOrderInfo, shortOrderInfo};
        }
    };
}