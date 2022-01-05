//
// Created by 裴沛东 on 2021/12/27.
//

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
Socket Socket::accept(InetAddress *client_addr) const {
     int client_fd = ::accept(fd, (sockaddr*)&client_addr->addr, &client_addr->addr_len);
     errif(client_fd == -1,"socket accept failed");
     return Socket(client_fd,client_addr);
}
Socket::~Socket() {
    if(fd == -1) {
        close(fd);
        fd = -1;
    }
    delete addr;
    addr = nullptr;

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
