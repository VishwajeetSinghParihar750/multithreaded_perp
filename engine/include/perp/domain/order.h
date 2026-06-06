#pragma once
#include <string>
#include <inttypes.h>
#include <types.h>
#include <unordered_map>

namespace DOMAIN
{

  class Order
  {

    // per symbol thread safe
    static std::unordered_map<TRADABLE_CURRENCY_SYMBOL, uint64_t> orderIdCounter;

  public:
    USER_ID userId;
    uint64_t price;
    uint64_t quantity;
    uint64_t margin;
    uint64_t filledQuantity;
    TRADABLE_CURRENCY_SYMBOL symbol;
    ORDER_STATUS status;
    SIDE side;
    ORDER_TYPE type;
    MARGIN_TYPE marginType;
    ORDER_ID orderId;

    Order(USER_ID userId, uint64_t price, uint64_t quantity,
          uint64_t margin, TRADABLE_CURRENCY_SYMBOL symbol,
          SIDE side, ORDER_TYPE type, MARGIN_TYPE marginType,
          ORDER_STATUS status = ORDER_STATUS::OPEN, uint64_t filledQuantity = 0)

        : userId(userId),
          price(price), quantity(quantity),
          margin(margin), filledQuantity(filledQuantity), symbol(symbol),
          status(status), side(side), type(type), marginType(marginType),
          orderId(std::to_string(int(symbol)).append(std::to_string(orderIdCounter[symbol]++)))
    {
    }
  };

} // namespace DOMAIN