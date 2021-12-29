//
// Created by 裴沛东 on 2021/12/27.
//

#ifndef TCPSERVER_EPOLL_H
#define TCPSERVER_EPOLL_H
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <cstdio>
#include "util.h"
#include "Channel.h"
#define MAX_EVENTS 1000
class Channel;
class Epoll {
private:
    int epollfd;
    epoll_event *events;
    std::vector<Channel*> ActiveEvents;
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,uint32_t option);
    bool poll(int timeout = -1);
    std::vector<Channel*>& getActiveEvents();
    void updateChannel(Channel *channel);
};

#endif //TCPSERVER_EPOLL_H
