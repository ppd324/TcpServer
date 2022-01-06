//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_CHANNEL_H
#define TCPSERVER_CHANNEL_H
#include <sys/epoll.h>
#include <functional>
#include "Epoll.h"
#include "EventLoop.h"
#include "Socket.h"
#include <memory>
class Epoll;
class EventLoop;
class Channel {
public:
    explicit Channel(std::shared_ptr<EventLoop> loop, std::shared_ptr<Socket> sock);
    ~Channel();
    void enableReading();
    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInpoll() const;
    void setInpoll();
    void setEvents(uint32_t events);
    void setCallback(std::function<void()> &_cb);
    void enableWriting();
    void enableDeleting();
    void enableListening();
    void handleEvent();
    void setNotUseThreadPool(bool flag);

private:
    std::shared_ptr<EventLoop> _loop;
    std::shared_ptr<Socket> sock;
    uint32_t events;
    uint32_t revents;
    bool inpoll;
    std::function<void()> callback;
    bool flag = false;

};
#endif //TCPSERVER_CHANNEL_H
