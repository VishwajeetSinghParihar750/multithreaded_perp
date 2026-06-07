#include <iostream>

#include "../include/perp/application/eventBus.h"
using namespace std;
class orderplaced
{
};
auto handler(const orderplaced &order)
{
}
int main()
{
    // auto eb = new EventBus();
    // eb->subscribe<orderplaced>(handler);

    // sw::redis::Redis redis("redis://localhost:6379");
    // string res = redis.xadd("random-redis-stream", "*", {"name", "value"});
    // std::cout << res << std::endl;
    std::cout
        << __cplusplus << '\n';
    cout << "hellow orld " << endl;
}
