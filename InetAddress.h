//
// Created by 裴沛东 on 2021/12/27.
//

#ifndef TCPSERVER_INETADDRESS_H
#define TCPSERVER_INETADDRESS_H
#include <arpa/inet.h>
class InetAddress {
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
private:

};
#endif //TCPSERVER_INETADDRESS_H
