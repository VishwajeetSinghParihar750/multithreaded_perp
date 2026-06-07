#pragma once

#include <unordered_map>

#include "eventBus.h"
#include "position.h"

#include "../event/event.h"

namespace DOMAIN
{
    class PositionManager
    {

        std::unordered_map<USER_ID, Position> positions;

        //

        EventBus &eventBus;

        //
        void applyTrade(const Trade &trade)
        {
            //
        }

    public:
        PositionManager(EventBus &eventBus_) : eventBus(eventBus_)
        {
            eventBus.subscribe<TradeCreated>([this](const TradeCreated &trade)
                                             { this->applyTrade(trade); });
        }
    };
}