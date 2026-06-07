#pragma once
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <inttypes.h>
#include <assert.h>
#include <typeindex>
#include <any>

using SubscriptionId = uint64_t;

class EventBus
{
    SubscriptionId subscriptionIdCounter = 0;

    std::unordered_map<std::type_index, std::unordered_set<SubscriptionId>> subscriptions;

    std::unordered_map<SubscriptionId, std::function<void(const std::any &)>> eventHandlers;
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
            eventHandlers[subId](event); // this event is converted to std::any
        }
    }

    template <typename Event>
    SubscriptionId subscribe(std::function<void(const Event &)> eventHandler)
    {
        auto newSubId = subscriptionIdCounter++;

        std::type_index type(typeid(Event));

        subscriptions[type].insert(newSubId);
        eventHandlers[newSubId] = [eventHandler](const std::any &anyEvent)
        {
            eventHandler(std::any_cast<const Event &>(anyEvent));
        };

        subscriptionEventType.emplace(newSubId, type);

        return newSubId;
    }

    void unsubscribe(SubscriptionId id)
    {

        assert(subscriptionEventType.contains(id));

        auto type = subscriptionEventType.at(id);
        subscriptions[type].erase(id);
        subscriptionEventType.erase(id);
        eventHandlers.erase(id);
    }
};