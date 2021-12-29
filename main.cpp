#include <iostream>
#include "Server.h"
#include "EventLoop.h"
int main() {
    auto loop = std::make_shared<EventLoop>();
    auto server = std::make_shared<Server>(loop,8888);
    loop->loop();
    return 0;
}
