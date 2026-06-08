#pragma once

#include "sw/redis++/redis++.h"

namespace INFRA
{

    //
    class Redis
    {
        sw::redis::Redis redis; // redis

        // results
        using Attrs = std::vector<std::pair<std::string, std::string>>;
        Attrs attrs = {{"f1", "v1"}, {"f2", "v2"}};

        using Item = std::pair<std::string, std::optional<Attrs>>;
        using ItemStream = std::vector<Item>;

        std::unordered_map<std::string, ItemStream> result;

    public:
        Redis() : redis("redis://localhost:6327") // cons will throw if error, which is what we want
        {
            // setup error handling for this redis client
        }

        auto getNext(std::string_view id)
        {

            // TODO : get start id from snapshot
            std::vector<int> response;

            // redis.xread("ENGINE_INPUT_STREAM", id, std::chrono::milliseconds(0), 10, std::inserter(result, result.end())); //
        }
        void send() {}
    };

};