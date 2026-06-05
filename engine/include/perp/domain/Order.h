#pragma once
#include <string>
#include <inttypes.h>
#include <types.h>

class Order
{
    DOMAIN::USER_ID userId;
    DOMAIN::ORDER_ID orderId;
    uint64_t price;
    uint64_t quantity;
    uint64_t margin;
    uint64_t filledQuantity;
    DOMAIN::TRADABLE_CURRENCY_SYMBOL symbol;
    DOMAIN::ORDER_STATUS status;
    DOMAIN::SIDE side;
    DOMAIN::ORDER_TYPE type;
    DOMAIN::MARGIN_TYPE marginType;
};