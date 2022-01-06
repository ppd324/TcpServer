//
// Created by 裴沛东 on 2021/12/28.
//

#include "Channel.h"

class EventLoop;
Channel::Channel(std::shared_ptr<EventLoop> loop, std::shared_ptr<Socket> sock): _loop(std::move(loop)),sock(std::move(sock)),events(0),revents(0),inpoll(false) {
}

Channel::~Channel() {

}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    _loop->updateChannel(this);
}

void Channel::enableListening() {
    events = EPOLLRDHUP |EPOLLET | EPOLLIN;
    _loop->updateChannel(this);
}



int Channel::getFd() const {
    return sock->get_fd();
}

uint32_t Channel::getEvents() const {
    return events;
}

uint32_t Channel::getRevents() const {
    return revents;
}

bool Channel::getInpoll() const {
    return inpoll;
}

void Channel::setInpoll() {
    inpoll = true;

}

void Channel::setEvents(uint32_t events) {
    revents = events;

}

void Channel::setCallback(std::function<void()> &_cb) {
    callback = _cb;

}

void Channel::handleEvent() {
    if(flag) {
        callback(); //直接执行
    }else {
        _loop->addTaskToQueue(callback); //加入线程队列
    }

}

void Channel::enableWriting() {
    events = EPOLLOUT | EPOLLET;
    _loop->updateChannel(this);

}

void Channel::enableDeleting() {
    _loop->deleteChannel(this);

}

void Channel::setNotUseThreadPool(bool flag) {
    this->flag  = flag;

}
