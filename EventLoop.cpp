//
// Created by 裴沛东 on 2021/12/28.
//

#include "EventLoop.h"
EventLoop::EventLoop():ep(nullptr),quit(false),timer( new Timer(60000)),TimeOutFlag(false){
    ep = new Epoll();

}

EventLoop::~EventLoop() {
    delete ep;

}
void EventLoop::loop() {
    while(!quit) {
        int ret = -1;
        if(TimeOutFlag) {
             ret = timer->GetNextTick();
        }
        if(ep->poll(ret)) {
            std::vector<Channel*> active = std::move(ep->getActiveEvents());
            for(auto & it : active) {
                it->handleEvent();

            }
        }

    }

}
void EventLoop::updateChannel(Channel *channel) {
    ep->updateChannel(channel);

}
void EventLoop::addTaskToQueue(std::function<void()> &task) {
    //threadPool->addTask(task);

}

void EventLoop::deleteChannel(Channel *channel) {
    ep->deleteChannel(channel);

}

void EventLoop::setTimeOut(bool flag) {
    TimeOutFlag = flag;


}

EventLoop::EventLoop(bool flag):ep(nullptr),quit(false),timer( new Timer(60000)),TimeOutFlag(flag){
    ep = new Epoll();

}
