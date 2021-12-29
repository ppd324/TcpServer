//
// Created by 裴沛东 on 2021/12/27.
//

#include "InetAddress.h"
#include <cstring>
InetAddress::InetAddress(const char *ip, uint16_t port) {
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr_len = sizeof(addr);
}
InetAddress::InetAddress():addr_len(sizeof(addr)) {
    bzero(&addr,sizeof(addr));
}
InetAddress::~InetAddress() {

}