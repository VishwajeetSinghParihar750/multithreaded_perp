#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <any>
#include <assert.h>

#include "order.h"
#include "event/event.h"
#include "types.h"
#include "trade.h"
#include "eventBus.h"

namespace DOMAIN
{

    class Orderbook
    {

        using PriceLevel = std::list<std::unique_ptr<Order>>;
        using OrderPtr = PriceLevel::iterator;

        //
        EventBus &eventBus;
        MARKET_ID marketId;

        //
        std::priority_queue<PRICE, std::vector<PRICE>, std::greater<PRICE>> asksPrices;
        std::priority_queue<PRICE> bidsPrices;

        std::unordered_map<PRICE, PriceLevel> askPriceLevels;
        std::unordered_map<PRICE, PriceLevel> bidPriceLevels;

        std::unordered_map<ORDER_ID, OrderPtr> orders;

        //

        void matchOrders(const std::unique_ptr<Order> &order1, const std::unique_ptr<Order> &order2, PRICE margin1Required, PRICE margin2Required)
        {
            // emit trade
            auto tradePrice = std::min(order1->price, order2->price);
            auto tradeQuantity = std::min(order2->quantity - order2->filledQuantity, order1->quantity - order1->filledQuantity);

            order1->margin -= margin1Required;
            order2->margin -= margin2Required;
            //
        }

        template <typename OppostePricesType, typename OppostePriceLevelsType>
        void matchAgainstbook(const std::unique_ptr<Order> &order, const OppostePricesType &oppositePrices,
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
                // could ask risk engine for required margin for trade again from both users
                // if can trade we need required margin to deduct

                // match with best price level orders
                auto bestPriceLevel = oppositePriceLevels[bestOppositePrice];

                for (auto it = bestPriceLevel.begin(); it != bestPriceLevel.end() && order->filledQuantity < order->quantity; it = bestPriceLevel.erase(it))
                {
                    // trade
                    matchOrders(*it, order, 100, 100);
                }

                // remove level if needed
                if (bestPriceLevel.empty())
                {
                    oppositePriceLevels.erase(bestOppositePrice);
                    oppositePrices.pop();
                }
            }
        }

        void match(const std::unique_ptr<Order> &order)
        {

            if (order->side == SIDE::LONG)
                matchAgainstbook(order, asksPrices, askPriceLevels);
            else
                matchAgainstbook(order, bidsPrices, bidPriceLevels);
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

    public:
        Orderbook(EventBus &eventBus_, MARKET_ID marketId_) : eventBus(eventBus_), marketId(marketId_) {}

        void placeOrder(std::unique_ptr<Order> order)
        {
            match(order);
            if (order->type == ORDER_TYPE::LIMIT && order->filledQuantity < order->quantity)
                sitOnBook(std::move(order));
        }
    };

} // namespace DOMAIN