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
#include "riskEngine.h"
#include "event/event.h"
#include "types.h"
#include "trade.h"
#include "eventBus.h"
namespace DOMAIN
{

    class Orderbook
    {

        using Depth = std::tuple<PRICE, QUANTITY>;
        using PriceLevel = std::list<std::unique_ptr<Order>>;
        using OrderPtr = PriceLevel::iterator;

        //
        RiskEngine &riskEngine;
        TradeFactory &tradeFactory;
        EventBus &eventBus;
        MARKET_ID marketId;

        //
        std::priority_queue<PRICE, std::vector<PRICE>, std::greater<PRICE>> asksPrices;
        std::priority_queue<PRICE> bidsPrices;

        std::unordered_map<PRICE, std::tuple<QUANTITY, PriceLevel>> askPriceLevels;
        std::unordered_map<PRICE, std::tuple<QUANTITY, PriceLevel>> bidPriceLevels;

        std::unordered_map<ORDER_ID, OrderPtr> orders;
        //

        void cancelOrderStatusAndEmit(const std::unique_ptr<Order> &order)
        {
            order->status = ORDER_STATUS::CANCELLED;

            OrderCancelled event{order->orderId};
            eventBus.emit<OrderCancelled>(event);
        }

        void matchOrders(const std::unique_ptr<Order> &order1, const std::unique_ptr<Order> &order2, PRICE margin1Required, PRICE margin2Required)
        {
            PRICE tradePrice = std::min(order1->price, order2->price);
            QUANTITY tradeQuantity = std::min(order2->quantity - order2->filledQuantity, order1->quantity - order1->filledQuantity);

            assert(tradeQuantity > 0);

            order1->margin -= margin1Required;
            order2->margin -= margin2Required;

            order1->filledQuantity += tradeQuantity;
            order2->filledQuantity += tradeQuantity;

            order1->status = (order1->filledQuantity == order1->quantity ? ORDER_STATUS::FILLED : ORDER_STATUS::PARTIALLY_FILLED);
            order2->status = (order2->filledQuantity == order2->quantity ? ORDER_STATUS::FILLED : ORDER_STATUS::PARTIALLY_FILLED);

            // emit trade
            auto order1Info = TradeOrderInfo{
                order1->userId,
                order1->orderId,
                order1->filledQuantity,
                order1->quantity,
                order1->status};
            auto order2Info = TradeOrderInfo{
                order1->userId,
                order1->orderId,
                order1->filledQuantity,
                order1->quantity,
                order1->status};

            TradeCreated tradeEvent = tradeFactory.create(
                tradePrice,
                tradeQuantity,
                marketId,
                order1->side == SIDE::LONG ? order1Info : order2Info,
                order1->side == SIDE::SHORT ? order1Info : order2Info);

            eventBus.emit<TradeCreated>(tradeEvent);
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

                // match with best price level orders
                auto bestPriceLevel = oppositePriceLevels[bestOppositePrice];

                for (auto it = bestPriceLevel.begin(); it != bestPriceLevel.end() && order->filledQuantity < order->quantity;)
                {
                    // could ask risk engine for required margin for trade again from both users
                    // if can trade we need required margin to deduct
                    auto [marginRequired1, marginRequired2] = riskEngine.evaluateTrade(*it, order);

                    if (marginRequired1 > it->margin)
                    {
                        cancelOrderStatusAndEmit(it);

                        // remove from orderbook
                        orders.erase(it->orderId);
                        it = bestPriceLevel.erase(it);

                        continue;
                    }
                    if (marginRequired2 > order->margin)
                    {
                        cancelOrderStatusAndEmit(order);
                        return;
                    }

                    // trade
                    matchOrders(*it, order, marginRequired1, marginRequired2);

                    if (it->filledQuantity == it->quantity)
                    {
                        // remove from orderbook
                        orders.erase(it->orderId);
                        it = bestPriceLevel.erase(it)
                    }
                    // else it should break anyways
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

        template <typename PriceLevelsType>
        void sitOnBook(std::unique_ptr<Order> order, PriceLevelsType &priceLevels)
        {
            assert(order->quantity > order->filledQuantity);
            assert(!orders.contains(order->orderId));

            auto orderId = order->orderId;
            auto price = order->price;

            if (!priceLevels.contains(price))
                asksPrices.push(price);

            auto &[qty, priceLevel] = priceLevels[price];
            qty += order->quantity - order->filledQuantity;
            priceLevel.push_back(std::move(order));
        }

    public:
        Orderbook(RiskEngine &riskEngine_, TradeFactory tradeFactory_, EventBus &eventBus_, MARKET_ID marketId_)
            : riskEngine(riskEngine_), tradeFactory(tradeFactory_), eventBus(eventBus_), marketId(marketId_) {}

        void placeOrder(std::unique_ptr<Order> order)
        {
            match(order);
            if (order->status != ORDER_STATUS::CANCELLED && order->type == ORDER_TYPE::LIMIT && order->filledQuantity < order->quantity)
                sitOnBook(std::move(order), order->side == SIDE::LONG ? bidPriceLevels : askPriceLevels);
        }

        void cancelOrder(ORDER_ID orderId)
        {
            assert(orders.contains(orderId));

            auto orderPtr = orders[orderId];
            auto price = orderPtr->get()->price;

            if (orderPtr->get()->side == SIDE::LONG)
            {
                auto &[qty, priceLevel] = bidPriceLevels[price];

                qty -= orderPtr->get()->quantity - orderPtr->get()->filledQuantity;
                priceLevel.erase(orderPtr);

                if (qty == 0)
                    bidPriceLevels.erase(price);
            }
            else
            {
                auto &[qty, priceLevel] = askPriceLevels[price];

                qty -= orderPtr->get()->quantity - orderPtr->get()->filledQuantity;
                priceLevel.erase(orderPtr);

                if (qty == 0)
                    askPriceLevels.erase(price);
            }

            orders.erase(orderId);

            // emit event
            OrderCancelled event{orderId};
            eventBus.emit<OrderCancelled>(event);
        }

        // [ long, short ] depths
        std::tuple<std::vector<Depth>, std::vector<Depth>> getDepth() const
        {
            std::vector<Depth> longDepths, shortDepths;
            for (const auto &[price, levelInfo] : askPriceLevels)
                longDepths.emplace_back(price, std::get<0>(levelInfo));

            for (const auto &[price, levelInfo] : bidPriceLevels)
                shortDepths.emplace_back(price, std::get<0>(levelInfo));

            return {longDepths, shortDepths};
        }
    };

} // namespace DOMAIN