#pragma once

#include "types.h"

namespace DOMAIN
{

    struct TradeOrderInfo
    {
        USER_ID userId;
        ORDER_ID orderId;
        QUANTITY filledQuantity;
        QUANTITY totalQuantity;
        ORDER_STATUS status;
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
}