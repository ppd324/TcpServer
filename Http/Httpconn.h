//
// Created by 裴沛东 on 2021/12/31.
//

#ifndef TCPSERVER_HTTPCONN_H
#define TCPSERVER_HTTPCONN_H
#include "../Connection.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
class Httpconn :public Connection {
public:
    Httpconn(std::shared_ptr<EventLoop>  _loop, const std::shared_ptr<Socket>& _socket);
    ~Httpconn();
    size_t read();
    size_t write();
    bool process();

private:
    HttpRequest httpRequest_;
    HttpResponse httpResponse_;



};


#endif //TCPSERVER_HTTPCONN_H
