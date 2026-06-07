#pragma once

#include "types.h"

namespace DOMAIN
{
    struct Trade
    {

        USER_ID userId;
        PRICE price;
        QUANTITY quantity;
        PRICE margin;
        QUANTITY filledQuantity;
        MARKET_ID symbol;
        ORDER_STATUS status;
        SIDE side;
        ORDER_TYPE type;
        MARGIN_TYPE marginType;
        TRADE_ID tradeId;
    };
}