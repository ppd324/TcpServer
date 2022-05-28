//
// Created by 裴沛东 on 2021/12/27.
//
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include "Socket.h"
Socket::Socket():fd(-1),addr(nullptr){
    fd = socket(AF_INET,SOCK_STREAM,0);
    errif(fd == -1,"create socket failed");
}

Socket::Socket(enum protocol pro):fd(-1),addr(nullptr) {
    switch(pro) {
        case protocol::TCP :
            fd = socket(AF_INET,SOCK_STREAM,0);
            break;
        case protocol::UDP:
            fd = socket(AF_INET,SOCK_DGRAM,0);
            break;
    }
    errif(fd == -1,"create socket failed");
}
Socket::Socket(int fd):fd(fd),addr(nullptr){
    errif(fd == -1,"create socket failed");
}
Socket::Socket(int fd, InetAddress *addr):fd(fd),addr(addr) {
    errif(fd == -1,"create socket failed");

}
void Socket::bind(InetAddress *addr) {
    this->addr = addr;
    errif(::bind(fd,reinterpret_cast<struct sockaddr*>(&addr->addr),addr->addr_len) == -1,"socket bind failed");

}
void Socket::listen() const {
    errif(::listen(fd,SOMAXCONN) == -1,"socket listen failed");
}
std::shared_ptr<Socket> Socket::accept() const {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof(client_addr);
    int accept_fd = 0;
    int client_fd = ::accept(fd, (sockaddr*)&client_addr, &client_addr_len);
    if(client_fd < 0) return nullptr;
    LOG_INFO("new client fd is %d, ip is %s ,port is %d", client_fd, inet_ntoa(client_addr.sin_addr),
             ntohs(client_addr.sin_port));
    return std::make_shared<Socket>(client_fd);
}
Socket::~Socket() {
    std::cout<<"socket deconstruct"<<std::endl;
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
    if(addr != nullptr) {
        delete addr;
        addr = nullptr;
    }

}
int Socket::get_fd() const {
    return fd;
}
void Socket::set_fd(int fd) {
    this->fd = fd;
}
void Socket::setnonblocking() const {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
void Socket::setsockopt() {
    int opt = 1;
    errif(::setsockopt(fd,SOL_SOCKET, SO_REUSEADDR,(void*)&opt,sizeof(opt)) < 0,"reuse socket error");
}

Socket::Socket(const std::string& ip, int port):addr(new InetAddress(ip.c_str(),port)) {
    fd = socket(AF_INET,SOCK_STREAM,0);
    errif(fd == -1,"create socket failed");
    int opt = 1;
    errif(::setsockopt(fd,SOL_SOCKET, SO_REUSEADDR,(void*)&opt,sizeof(opt)) < 0,"reuse socket error");
    errif(::bind(fd,reinterpret_cast<struct sockaddr*>(&addr->addr),addr->addr_len) == -1,"socket bind failed");
    errif(::listen(fd,SOMAXCONN) == -1,"socket listen failed");

}
