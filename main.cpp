#include <iostream>
#include "Server.h"
#include "EventLoop.h"
using namespace Log;
int main() {
    initLogLevel();
    Logger::setLogFmtType(Logger::STRING);
    Logger::setLogMode(OUTPUT_CONSOLE);
    auto loop = std::make_shared<EventLoop>();
    auto server = std::make_shared<Server>(loop,8888);
    loop->loop();
    return 0;
}
