#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>
#include <typeindex>

using SubscriptionId = uint64_t;

class EventBus
{

    SubscriptionId subscriptionIdCounter = 0;

    std::unordered_map<std::type_index, std::unordered_set<SubscriptionId>> subscriptions;

    std::unordered_map<SubscriptionId, std::function<void()>> eventHandlers;
    std::unordered_map<SubscriptionId, std::type_index> subscriptionEventType;

public:
    template <typename Event>
    void emit(const Event &event)
    {
        std::type_index type(typeid(Event));

        if (!subscriptions.contains(type))
            return;

        for (auto subId : subscriptions[type])
        {
            assert(eventHandlers.contains(subId));
            eventHandlers[subId](event);
        }
    }

    template <typename Event>
    void subscribe(const Event &event, const std::function<void(
                                           Event)> &eventHandler)
    {
        auto newSubId = subscriptionIdCounter++;

        std::type_index type(typeid(Event));

        subscriptions[type].insert(newSubId);
        eventHandlers[newSubId] = eventHandler;
        subscriptionEventType[newSubId] = type;
    }

    void unsubscribe(SubscriptionId id)
    {

        assert(subscriptionEventType.contains(id));

        std::type_index type = subscriptionEventType[id];

        subscriptions[subscriptionEventType[id]].erase(id);
        subscriptionEventType.erase(id);
        eventHandlers.erase(id);
    }
};