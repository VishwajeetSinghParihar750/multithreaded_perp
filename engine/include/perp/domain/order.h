#pragma once
#include <string>
#include <inttypes.h>
#include <types.h>
#include <unordered_map>

namespace DOMAIN
{

  struct Order
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
    ORDER_ID orderId;
  };

} // namespace DOMAIN