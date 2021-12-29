//
// Created by 裴沛东 on 2021/12/28.
//

#include "EventLoop.h"
EventLoop::EventLoop():ep(nullptr),quit(false),threadPool(new ThreadPool()){
    ep = new Epoll();

}

EventLoop::~EventLoop() {
    delete ep;

}
void EventLoop::loop() {
    while(!quit) {
        if(ep->poll()) {
            std::vector<Channel*> active = ep->getActiveEvents();
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
    threadPool->addTask(task);

}
