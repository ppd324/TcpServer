//
// Created by 裴沛东 on 2021/12/27.
//
#define MAX_EVENTS 20
#include "Epoll.h"
Epoll::Epoll():epollfd(-1),events(nullptr){
    epollfd = epoll_create(1);
    errif(epollfd == -1,"epoll_create failed");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);


}
void Epoll::addFd(int fd, uint32_t option) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = option;
    errif(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}
Epoll::~Epoll() {
    if(epollfd != -1) {
        close(epollfd);
        epollfd = -1;
    }
    delete[] events;

}
bool Epoll::poll(int timeout) {
    ActiveEvents.clear();
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        Channel *channel = (Channel*)events[i].data.ptr;
        ActiveEvents.emplace_back(channel);
    }
    return true;
}

std::vector<Channel*> & Epoll::getActiveEvents() {
    return ActiveEvents;
}

void Epoll::updateChannel(Channel *channel) {
    int fd = channel->getFd();
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.data.ptr = channel;
    event.events = channel->getEvents();
    //如果不在树上
    if(!channel->getInpoll()) {
        errif(epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event) == -1,"epoll add event failed");
        channel->setInpoll();
    } else {
        errif(epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event) == -1,"epoll modify event failed");
    }

}

void Epoll::deleteChannel(Channel *channel) {
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.data.ptr = channel;
    event.events = channel->getEvents();
    if(channel->getInpoll()) {
        errif(epoll_ctl(epollfd,EPOLL_CTL_DEL,channel->getFd(),&event) == -1,"epoll delete event failed");
    }

}

void Epoll::modifyChannel(Channel *channel) {


}
