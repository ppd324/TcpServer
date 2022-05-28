//
// Created by 裴沛东 on 2021/12/27.
//

#ifndef TCPSERVER_EPOLL_H
#define TCPSERVER_EPOLL_H
#define MAX_EVENTS 1024
#include <vector>
#include <sys/epoll.h>
#include "Channel.h"
class Channel;
class Epoll {
private:
    int epollfd;
    epoll_event *events;
public:
    std::vector<Channel*> ActiveEvents;
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,uint32_t option);
    bool poll(int timeout = -1);
    std::vector<Channel*>& getActiveEvents();
    void updateChannel(Channel *channel);
    void deleteChannel(Channel *channel);
    void modifyChannel(Channel *channel);
};

#endif //TCPSERVER_EPOLL_H
