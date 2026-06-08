#pragma once

#include <unordered_map>
#include <memory>

#include "eventBus.h"
#include "position.h"
#include "riskEngine.h"

#include "../event/event.h"

namespace DOMAIN
{
    class PositionManager
    {

        std::unordered_map<USER_ID, std::unique_ptr<Position>> positions;

        //

        EventBus &eventBus;
        RiskEngine &riskEngine;

        void applyTrade(const Trade &trade, SIDE side)
        {
            auto &price = trade.price;
            auto &qty = trade.filledQuantity;
            // apply trade
            auto curSideTrade = side == SIDE::LONG ? trade.longOrderInfo : trade.shortOrderInfo;
            if (!positions.contains(curSideTrade.userId))
            {
                positions[curSideTrade.userId] = std::make_unique<Position>(Position{
                    curSideTrade.userId,
                    price,
                    qty,
                    SIDE::LONG,
                    trade.marketId,
                    curSideTrade.margin,
                    curSideTrade.marginType,
                    0,
                });
            }
            else
            {
                auto &position = positions[curSideTrade.userId];

                if (position->side == SIDE::LONG)
                {

                    auto priceQtyProductSum = position->price * position->quantity + price * qty;
                    auto weighedAvgPrice = priceQtyProductSum / (position->quantity + qty);

                    position->price = weighedAvgPrice;
                    position->quantity += qty;
                    position->margin += curSideTrade.margin;
                }
                else
                {
                    // do pnl
                    int64_t ogAmountSpent = position->price * std::min(position->quantity, qty);
                    int64_t gettingAmount = price * std::min(position->quantity, qty);

                    int64_t pnl = (gettingAmount - ogAmountSpent) * (side == SIDE::LONG ? 1 : -1);
                    PRICE releasedMargin = 0;

                    position->margin += curSideTrade.margin;
                    if (qty < position->quantity)
                    {
                        // price will stay same
                        position->quantity -= qty;
                    }
                    else if (qty > position->quantity)
                    {
                        position->price = price;
                        position->quantity = qty - position->quantity;
                    }
                    else
                    {
                        // release all margin
                        releasedMargin = position->margin;
                        position->margin = 0;
                        position->quantity = 0;
                    }

                    //
                    eventBus.emit<userPnlCreated>(userPnlCreated{position->userId, pnl, releasedMargin});
                }
            }

            // update liquidation price
            Position &positionToUpdate = side == SIDE::LONG ? *positions[trade.longOrderInfo.userId] : *positions[trade.shortOrderInfo.userId];
            positionToUpdate.liquidationPrice = riskEngine.getLiquidationPrice(positionToUpdate);
        }

    public:
        PositionManager(EventBus &eventBus_, RiskEngine &riskEngine_) : eventBus(eventBus_), riskEngine(riskEngine_)
        {
            eventBus.subscribe<TradeCreated>([this](const TradeCreated &trade)
                                             { 
                                            this->applyTrade(trade, SIDE::LONG);
                                            this->applyTrade(trade,SIDE::SHORT); });
        }

        STATUS::StatusOr<Position> getPosition(USER_ID userId) const
        {
            if (!positions.contains(userId))
                return std::unexpected(STATUS::Status(STATUS::StatusCode::kNotFound));
            return *positions.at(userId);
        }

        auto applyFunding() {}
    };
}