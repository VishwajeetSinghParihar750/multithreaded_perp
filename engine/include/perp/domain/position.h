#pragma once

#include "types.h"
namespace DOMAIN
{
    class Position
    {
        USER_ID userId;
        PRICE price;
        QUANTITY quantity;
        SIDE side;
        MARKET_ID marketId;
        PRICE margin;
        MARGIN_TYPE marginType;
    };
} // namespace DOMAIN