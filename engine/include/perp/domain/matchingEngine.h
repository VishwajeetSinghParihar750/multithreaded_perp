#pragma once
#include <tuple>
#include <vector>
#include <memory>

#include "trade.h"
#include "event/event.h"
#include "order.h"
#include "orderbook.h"
#include "event/event.h"

namespace DOMAIN
{
    class MatchingEngine
    {
        Orderbook &orderbook;

    public:
        MatchingEngine(Orderbook &orderbook) : orderbook(orderbook) {}

        std::vector<Event> placeOrder(std::unique_ptr<DOMAIN::Order> order)
        {
            //
        }
    };
}