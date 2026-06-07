#pragma once
#include <tuple>
#include <vector>
#include <memory>
#include <any>

#include "trade.h"
#include "event/event.h"
#include "order.h"
#include "orderbook.h"
#include "event/event.h"

namespace DOMAIN
{
    class MatchingEngine
    {
        using SymbolOrderbook = std::unordered_map<MARKET_ID, std::shared_ptr<Orderbook>>;
        SymbolOrderbook &orderbooks;

        std::vector<std::any> match(const std::unique_ptr<Order> &order)
        {
        }
        void sitOnBook(std::unique_ptr<Order> order)
        {
            orderbooks[order->symbol]->sitOnBook(std::move(order));
        }

    public:
        MatchingEngine(SymbolOrderbook &orderbooks) : orderbooks(orderbooks) {}

        std::vector<std::any> placeOrder(std::unique_ptr<Order> order)
        {
            auto events = match(order);

            if (order->type == ORDER_TYPE::LIMIT && order->filledQuantity < order->quantity)
                sitOnBook(std::move(order));

            return events;
        }
    };
}