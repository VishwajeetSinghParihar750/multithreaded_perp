#include <iostream>
#include "order.pb.h"
#include <sw/redis++/redis++.h>

using namespace std;

int main()
{

    sw::redis::Redis redis("redis://localhost:6379");
    string res = redis.xadd("random-redis-stream", "*", {"name", "value"});
    std::cout << res << std::endl;

    Order order;
    order.set_name("sahil");

    cout << "hellow orld " << endl;
}
