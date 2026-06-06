#pragma once
#include <tuple>
#include <vector>
#include "trade.h"
#include "event/event.h"
#include "order.h"
namespace DOMAIN

{
    using event = int;
    class MatchingEngine
    {

    public:
        std::vector<Event> placeOrder(DOMAIN::Order order)
        {
        }
    };
}