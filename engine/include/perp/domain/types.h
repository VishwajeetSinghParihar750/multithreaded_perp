#pragma once

#include <inttypes.h>
#include <string>

namespace DOMAIN
{

    using USER_ID = std::string;
    using ORDER_ID = std::string;

    using PRICE = uint64_t;
    using QUANTITY = uint64_t;

    enum class SIDE
    {
        LONG,
        SHORT
    };
    enum class TRADABLE_CURRENCY_SYMBOL
    {
        USD,
        BTCUSD,
        ETHUSD,
        SOLUSD
    };

    enum class ORDER_TYPE
    {
        MARKET,
        LIMIT
    };

    enum class MARGIN_TYPE
    {
        ISOLATED,
        CROSS
    };
    enum class ORDER_STATUS
    {
        OPEN,
        FILLED,
        PARTIALLY_FILLED,
        CANCELLED
    };

}