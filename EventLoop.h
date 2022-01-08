//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_EVENTLOOP_H
#define TCPSERVER_EVENTLOOP_H
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
class Channel;
class Epoll;
class ThreadPool;
class EventLoop{
private:
    //std::shared_ptr<ThreadPool> threadPool;
    Epoll *ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *channel);
    void deleteChannel(Channel *channel);
    void addTaskToQueue(std::function<void()> &task);

};

#endif //TCPSERVER_EVENTLOOP_H
