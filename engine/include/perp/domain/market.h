#pragma once
#include <string>
namespace DOMAIN
{
    class Market
    {
    public:
        struct MarketId
        {
            std::string value;
        };

    private:
        MarketId marketId;
    };
}