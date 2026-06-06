#pragma once
#include <tuple>
#include <vector>
#include "Trade.h"
#include "Order.h"
namespace DOMAIN

{
    using event = int;
    using placeOrderResponse = std::tuple<std::vector<event>, std::vector<Trade>, std::vector<Order>, std::vector<Order>>;
    class MatchingEngine
    {

    public:
        placeOrderResponse placeOrder(DOMAIN::Order order)
        {
        }
    };
}