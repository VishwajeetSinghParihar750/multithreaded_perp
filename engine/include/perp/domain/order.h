#pragma once

#include <memory>
#include <string>
#include <inttypes.h>
#include <types.h>
#include <unordered_map>

namespace DOMAIN
{

  struct Order
  {
    ORDER_ID orderId;
    USER_ID userId;
    PRICE price;
    QUANTITY quantity;
    PRICE margin;
    QUANTITY filledQuantity;
    MARKET_ID marketId;
    ORDER_STATUS status;
    SIDE side;
    ORDER_TYPE type;
    MARGIN_TYPE marginType;
  };

  class OrderFactory
  {

    using IdCounter = uint64_t;
    MARKET_ID marketId;
    IdCounter orderIdCounter = 0;

    ORDER_ID getNextOrderId()
    {
      return marketId.append(std::to_string(orderIdCounter++));
    }

  public:
    OrderFactory(MARKET_ID marketId_) : marketId(marketId_) {}

    std::unique_ptr<Order> createUniquePtr(USER_ID userId, PRICE price, QUANTITY quantity, PRICE margin, MARKET_ID marketId,
                                           SIDE side, ORDER_TYPE type, MARGIN_TYPE marginType, QUANTITY filledQuantity = 0, ORDER_STATUS status = ORDER_STATUS::OPEN)
    {
      return std::make_unique<DOMAIN::Order>(
          DOMAIN::Order{getNextOrderId(),
                        userId,
                        price, quantity, margin,
                        filledQuantity, marketId, status, side, type,
                        marginType});
    }
    Order create(USER_ID userId, PRICE price, QUANTITY quantity, PRICE margin, MARKET_ID marketId,
                 SIDE side, ORDER_TYPE type, MARGIN_TYPE marginType, QUANTITY filledQuantity = 0, ORDER_STATUS status = ORDER_STATUS::OPEN)
    {

      return Order{getNextOrderId(),
                   userId,
                   price, quantity, margin,
                   filledQuantity, marketId, status, side, type,
                   marginType};
    }
  };
} // namespace DOMAIN