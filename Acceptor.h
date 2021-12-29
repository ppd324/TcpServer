//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_ACCEPTOR_H
#define TCPSERVER_ACCEPTOR_H
#include "Socket.h"
#include "Channel.h"
class Acceptor{
private:
    std::shared_ptr<EventLoop> _loop;
    std::shared_ptr<Socket> _sock;
    std::shared_ptr<Channel> _channel;
    std::function<void(std::shared_ptr<Socket>)> newConnectionCallback;
public:
    Acceptor(std::shared_ptr<EventLoop>& loop,int port);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(std::shared_ptr<Socket>)> callback);
};
#endif //TCPSERVER_ACCEPTOR_H
