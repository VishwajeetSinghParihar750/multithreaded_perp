#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>

enum class EventType
{
};
class Event
{
public:
    EventType type;
};

using EventHandler = std::function<void(Event)>;

using SubscriptionId = uint64_t;
class EventBus
{

    SubscriptionId subscriptionIdCounter = 0;
    std::unordered_map<EventType, std::unordered_set<SubscriptionId>> subscriptions;
    std::unordered_map<SubscriptionId, EventHandler> eventHandlers;
    std::unordered_map<SubscriptionId, EventType> subscriptionEventType;

public:
    void emit(const Event &event)
    {
        if (!subscriptions.contains(event.type))
            return;

        for (auto subId : subscriptions[event.type])
        {
            assert(eventHandlers.contains(subId));
            eventHandlers[subId](event);
        }
    }

    void subscribe(const Event &event, const EventHandler &eventHandler)
    {
        auto newSubId = subscriptionIdCounter++;

        subscriptions[event.type].insert(newSubId);
        eventHandlers[newSubId] = eventHandler;
        subscriptionEventType[newSubId] = event.type;
    }

    void unsubscribe(SubscriptionId id)
    {

        assert(subscriptionEventType.contains(id));

        subscriptions[subscriptionEventType[id]].erase(id);
        subscriptionEventType.erase(id);
        eventHandlers.erase(id);
    }
};