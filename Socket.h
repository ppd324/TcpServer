//
// Created by 裴沛东 on 2021/12/27.
//

#ifndef TCPSERVER_SOCKET_H
#define TCPSERVER_SOCKET_H
#include "util.h"
#include "InetAddress.h"
enum class protocol {
    TCP,
    UDP

};
class Socket {
public:
    Socket();
    explicit Socket(int fd);
    Socket(int fd,InetAddress *addr);
    Socket(const std::string& ip,int port);
    Socket(protocol);
    ~Socket();
    void bind(InetAddress*);
    void listen() const;
    void setnonblocking() const;
    Socket accept(InetAddress* addr) const;
    void setsockopt();
    int get_fd() const;
    void set_fd(int fd);
private:
    InetAddress *addr;
    int fd;
};

#endif //TCPSERVER_SOCKET_H
