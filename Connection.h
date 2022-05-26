//
// Created by 裴沛东 on 2021/12/29.
//

#ifndef TCPSERVER_CONNECTION_H
#define TCPSERVER_CONNECTION_H
#pragma once
#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
class Channel;
class Socket;
class EventLoop;
class Buffer;
/**
 * 封装TCP连接类
 */

class Connection {
public:
    std::shared_ptr<Socket> socket;
    std::shared_ptr<Buffer> readBuffer;
    std::shared_ptr<Buffer> writeBuffer;
    std::function<void(std::shared_ptr<Socket>)> deleteConnetCallback;
public:
    Connection(std::shared_ptr<EventLoop>  _loop,std::shared_ptr<Socket>  _socket);
    virtual ~Connection();

    void echo(const std::shared_ptr<Socket>&);
    void setDeleteConnetCallback(std::function<void(std::shared_ptr<Socket>)> func);


    std::shared_ptr<Channel> channel;
    std::shared_ptr<EventLoop> loop;
};


#endif //TCPSERVER_CONNECTION_H
