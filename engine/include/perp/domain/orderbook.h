#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <assert.h>

#include "order.h"
#include "event/event.h"
#include "types.h"
#include "trade.h"

namespace DOMAIN
{

    class Orderbook
    {

        using PriceLevel = std::list<std::unique_ptr<Order>>;
        using OrderPtr = PriceLevel::iterator;

        //
        //
        //

        std::priority_queue<PRICE, std::vector<PRICE>, std::greater<PRICE>> asksPrices;
        std::priority_queue<PRICE> bidsPrices;

        std::unordered_map<PRICE, PriceLevel> askPriceLevels;
        std::unordered_map<PRICE, PriceLevel> bidPriceLevels;

        std::unordered_map<ORDER_ID, OrderPtr> orders;

        //
        //
        //
        std::unique_ptr<Trade> matchOrders(const std::unique_ptr<Order> &order1, const std::unique_ptr<Order> &order2)
        {
            //
        }

        template <typename OppostePricesType, typename OppostePriceLevelsType>
        std::vector<Event> matchAgainstbook(const std::unique_ptr<Order> &order, const OppostePricesType &oppositePrices,
                                            const OppostePriceLevelsType &oppositePriceLevels)
        {

            while (!oppositePrices() && order->filledQuantity < order->quantity)
            {

                // get best price
                auto bestOppositePrice = oppositePriceLevels.top();
                if (!oppositePrices.contains(bestOppositePrice))
                {
                    oppositePriceLevels.pop();
                    continue;
                }

                // best price match check
                bool canMatch = (order->side == SIDE::LONG ? curOrderPrice >= opOrderPrice;
                                                           : curOrderPrice < opOrderPrice);
                if (!canMatch)
                    break;

                // match with best price level orders
                auto bestPriceLevel = oppositePriceLevels[bestOppositePrice];

                for (auto it = bestPriceLevel.begin(); it != bestPriceLevel.end() && order->filledQuantity < order->quantity; it = bestPriceLevel.erase(it))
                {
                    // trades
                    matchOrders(*it, order);
                }

                // remove level if needed
                if (bestPriceLevel.empty())
                {
                    oppositePriceLevels.erase(bestOppositePrice);
                    oppositePrices.pop();
                }
            }
        }

    public:
        std::vector<Event> match(const std::unique_ptr<Order> &order)
        {

            if (order->side == SIDE::LONG)
                return matchAgainstbook(order, asksPrices, askPriceLevels);
            return matchAgainstbook(order, bidsPrices, bidPriceLevels);
        }

        void sitOnBook(std::unique_ptr<Order> order)
        {
            assert(order->quantity > order->filledQuantity);
            assert(!orders.contains(order->orderId));

            auto orderId = order->orderId;
            auto price = order->price;

            if (order->side == SIDE::LONG)
            {
                if (!bidPriceLevels.contains(price))
                    bidsPrices.push(price);

                bidPriceLevels[price].push_back(std::move(order));
                orders[orderId] = prev(bidPriceLevels[price].end());
            }
            else
            {
                if (!askPriceLevels.contains(price))
                    asksPrices.push(price);

                askPriceLevels[price].push_back(std::move(order));
                orders[orderId] = prev(askPriceLevels[price].end());
            }
        }
    };

} // namespace DOMAIN