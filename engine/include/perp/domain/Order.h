#pragma once
#include <string>
#include <inttypes.h>
#include <types.h>

namespace DOMAIN
{

    class Order
    {
    public:
        USER_ID userId;
        ORDER_ID orderId;
        uint64_t price;
        uint64_t quantity;
        uint64_t margin;
        uint64_t filledQuantity;
        TRADABLE_CURRENCY_SYMBOL symbol;
        ORDER_STATUS status;
        SIDE side;
        ORDER_TYPE type;
        MARGIN_TYPE marginType;

        Order(USER_ID userId, ORDER_ID orderId, uint64_t price, uint64_t quantity,
              uint64_t margin, uint64_t filledQuantity, TRADABLE_CURRENCY_SYMBOL symbol,
              ORDER_STATUS status, SIDE side, ORDER_TYPE type, MARGIN_TYPE marginType)
            : userId(userId), orderId(orderId), price(price), quantity(quantity),
              margin(margin), filledQuantity(filledQuantity), symbol(symbol),
              status(status), side(side), type(type), marginType(marginType) {}
    };
} // namespace DOMAIN