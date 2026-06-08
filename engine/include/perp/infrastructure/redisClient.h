#pragma once

#include "sw/redis++/redis++.h"
#include <list>
#include <vector>

namespace INFRA
{

    //
    std::string inputRedisStream = "ENGINE_INPUT_STREAM";

    // a single thraed will call these methods
    class Redis
    {
        sw::redis::Redis redis; // redis

        // results
        using Attrs = std::vector<std::pair<std::string, std::string>>;
        Attrs attrs = {{"f1", "v1"}, {"f2", "v2"}};

        using Item = std::pair<std::string, std::optional<Attrs>>;
        using ItemStream = std::list<Item>;

        std::unordered_map<std::string, ItemStream> result;

        // to send
        std::vector<std::pair<std::string, std::string>> attrs = {
            {"type", ""},
            {"payload", ""}};

    public:
        Redis() : redis("redis://localhost:6327") // cons will throw if error, which is what we want
        {
            // setup error handling for this redis client
        }

        auto getNext()
        {
            redis.xread(inputRedisStream, "0", std::chrono::milliseconds(0), 1, std::inserter(result, result.begin())); //
            auto toReturn = result[inputRedisStream].front();
            result[inputRedisStream].pop_front();

            return toReturn;
        }

        void send(std::string streamId, std::string messageType, std::string message)
        {
            attrs[0].second = messageType;
            attrs[1].second = message;

            redis.xadd(streamId, "*", attrs.begin(), attrs.end());
        }
    };

};