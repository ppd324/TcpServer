//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_EVENTLOOP_H
#define TCPSERVER_EVENTLOOP_H
#pragma once
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include "./Timer/Timer.h"
class Timer;
class Channel;
class Epoll;
class EventLoop{
private:
    //std::shared_ptr<ThreadPool> threadPool;
    Epoll *ep;
    bool quit;

    bool TimeOutFlag = false;


public:
    std::shared_ptr<Timer> timer;
    EventLoop();
    explicit EventLoop(bool flag);
    ~EventLoop();

    void loop();
    void updateChannel(Channel *channel);
    void deleteChannel(Channel *channel);
    void addTaskToQueue(std::function<void()> &task);
    void setTimeOut(bool flag);

};

#endif //TCPSERVER_EVENTLOOP_H
