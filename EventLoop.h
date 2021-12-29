//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_EVENTLOOP_H
#define TCPSERVER_EVENTLOOP_H
#include "Epoll.h"
#include "Channel.h"
class Channel;
class Epoll;
class EventLoop{
private:
    Epoll *ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *channel);

};

#endif //TCPSERVER_EVENTLOOP_H
