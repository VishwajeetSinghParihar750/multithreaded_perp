#pragma once

#include "sw/redis++/redis++.h"
#include <list>
#include <vector>

#include "../util/mpscQueue.h"

namespace INFRA
{

    using pairString = std::pair<std::string, std::string>;
    class Redis
    {
        sw::redis::Redis redis; // redis

        std::string inputRedisStream;
        // results
        using Attrs = std::vector<std::pair<std::string, std::string>>;
        Attrs attrs = {{"f1", "v1"}, {"f2", "v2"}};

        using Item = std::pair<std::string, std::optional<Attrs>>;
        using ItemStream = std::list<Item>;

        std::unordered_map<std::string, ItemStream> result;

        mpscQueue<pairString> outputQ;
        // to send
        static thread_local pairString response;

        void sendLoop()
        {
            //

            redis.xadd(streamId, "*", attrs.begin(), attrs.end());
        }

    public:
        Redis(std::string inputRedisStream_ = "ENGINE_INPUT_STREAM") : redis("redis://localhost:6327"), inputRedisStream(inputRedisStream_) // cons will throw if error, which is what we want
        {
            // setup error handling for this redis client
        }

        // this will be single threaded
        auto getNext()
        {
            redis.xread(inputRedisStream, "0", std::chrono::milliseconds(0), 1, std::inserter(result, result.begin())); //
            auto toReturn = result[inputRedisStream].front();
            result[inputRedisStream].pop_front();

            return toReturn;
        }

        // this will be multi threaded
        void send(std::string streamId, std::string messageType, std::string message)
        {

            // safe
            response.first = std::move(messageType);
            response.second = std::move(message);
        }
    };

    thread_local std::tuple<pairString, pairString> Redis::attrs = {
        {"type", ""},
        {"payload", ""}};

};