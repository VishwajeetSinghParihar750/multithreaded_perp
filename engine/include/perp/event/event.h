#pragma once

#include <vector>
#include <tuple>

#include "../domain/types.h"
#include "../domain/order.h"
#include "../domain/trade.h"
#include "../domain/types.h"

using OrderPlaced = DOMAIN::Order;
using TradeCreated = DOMAIN::Trade;
struct OrderCancelled
{
    DOMAIN::ORDER_ID orderId;
};

struct PositionUpdated
{
};

struct DepthUpdated
{
    DOMAIN::MARKET_ID symbol;
    std::vector<std::tuple<DOMAIN::PRICE, DOMAIN::QUANTITY>> bids;
    std::vector<std::tuple<DOMAIN::PRICE, DOMAIN::QUANTITY>> asks;
};
struct LastTradePriceUpdated
{
};

struct IndexPriceUpdated
{
};