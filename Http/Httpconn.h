//
// Created by 裴沛东 on 2021/12/31.
//

#ifndef TCPSERVER_HTTPCONN_H
#define TCPSERVER_HTTPCONN_H
#include <sys/uio.h>
#include "../Connection.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
class Httpconn :public Connection {
public:
    Httpconn(std::shared_ptr<EventLoop>  _loop, const std::shared_ptr<Socket>& _socket);
    ~Httpconn() override;
    bool handleEvent(std::shared_ptr<Socket> &_socket);
    size_t read(std::shared_ptr<Socket>& _socket);
    size_t write(std::shared_ptr<Socket>& _socket);
    bool process();
    int ToWriteBytes() {
        return iov_[0].iov_len + iov_[1].iov_len;
    }
    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
private:
    struct iovec iov_[2];
    int iovCnt_;
    HttpRequest httpRequest_;
    HttpResponse httpResponse_;



};


#endif //TCPSERVER_HTTPCONN_H
