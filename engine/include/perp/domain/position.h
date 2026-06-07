#pragma once

#include "types.h"
namespace DOMAIN
{
    struct Position
    {
        USER_ID userId;
        PRICE price;
        QUANTITY quantity;
        SIDE side;
        MARKET_ID marketId;
        PRICE margin;
        MARGIN_TYPE marginType;
        PRICE liquidationPrice;
    };
} // namespace DOMAIN