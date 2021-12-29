//
// Created by 裴沛东 on 2021/12/27.
//

#ifndef TCPSERVER_SOCKET_H
#define TCPSERVER_SOCKET_H
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"
#include "InetAddress.h"
enum class protocol {
    TCP,
    UDP

};
class InetAddress;
class Socket {
public:
    Socket();
    Socket(int fd);
    Socket(protocol);
    ~Socket();
    void bind(InetAddress*);
    void listen();
    void setnonblocking();
    Socket accept(InetAddress* addr);
    void setsockopt();
    int get_fd();
    void set_fd(int fd);
private:
    InetAddress *addr;
    int fd;
};

#endif //TCPSERVER_SOCKET_H
