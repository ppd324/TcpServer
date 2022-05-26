//
// Created by ppd on 2022/1/11.
//
#pragma once
#ifndef TCPSERVER_TIMER_H
#define TCPSERVER_TIMER_H
#include <unordered_map>
#include <chrono>
#include <functional>
#include <queue>
//#include "../Http/Httpconn.h"
class Httpconn;

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode {
    TimeStamp expires;
    TimeoutCallBack cb;
};

struct compare {
    bool operator() (const std::shared_ptr<TimerNode> &ptr1,const std::shared_ptr<TimerNode> &ptr2) {
        return ptr1->expires < ptr2->expires;

    }
};

class Timer {
public:
    void add(std::shared_ptr<Httpconn> &httpconn, const TimeoutCallBack& cb);
    void update(std::shared_ptr<Httpconn> &httpconn);
    int GetNextTick();
    void tick();
    explicit Timer(size_t timeout);
    void deleteConn(std::shared_ptr<Httpconn> &conn);
    ~Timer();

private:
    std::priority_queue<std::shared_ptr<TimerNode>,std::vector<std::shared_ptr<TimerNode>>,compare> _heap;
    std::unordered_map<std::shared_ptr<Httpconn>,std::shared_ptr<TimerNode>> _ref;
    size_t _timeout;

};


#endif //TCPSERVER_TIMER_H
