//
// Created by ppd on 2022/1/11.
//
#include "../Http/Httpconn.h"
#include "Timer.h"

/**
 * 对连接添加超时定时器
 * @param httpConn
 * @param timeOut
 * @param cb
 */
void Timer::add(std::shared_ptr<Httpconn> &httpConn, const TimeoutCallBack &cb) {
    if(_ref.count(httpConn ) == 0) { //新节点
        std::shared_ptr<TimerNode> timer(new TimerNode{Clock::now() + MS(_timeout),cb});
        _ref.insert(std::make_pair(httpConn, timer));
        _heap.push(timer);
    }else {
        std::shared_ptr<TimerNode> timer = _ref[httpConn];
        timer->expires = Clock::now() + MS(_timeout);
        timer->cb = cb;
    }

}
/***
 * 清除超时多余连接
 */
void Timer::tick() {
    if(_heap.empty()) {
       return;
    }
    while(!_heap.empty()) {
        std::shared_ptr<TimerNode> timer = _heap.top();
        if(std::chrono::duration_cast<MS>(timer->expires - Clock::now()).count() > 0) {
            break;
        }
        timer->cb();
        _heap.pop();
    }
}
int Timer::GetNextTick() {
    tick();
    long res = -1;
    if(!_heap.empty()) {
        res = std::chrono::duration_cast<MS>(_heap.top()->expires - Clock::now()).count();
        if(res < 0) { res = 0; }
    }
    return res;
}

Timer::~Timer() {
    _ref.clear();

}

Timer::Timer(size_t timeout):_timeout(timeout){

}

void Timer::update(std::shared_ptr<Httpconn> &httpconn) {
    std::shared_ptr<TimerNode> timer = _ref[httpconn];
    timer->expires = Clock::now() + MS(_timeout);

}
