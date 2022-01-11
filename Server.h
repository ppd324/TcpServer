//
// Created by 裴沛东 on 2021/12/28.
//

#ifndef TCPSERVER_SERVER_H
#define TCPSERVER_SERVER_H
#include <unordered_map>
#include <memory>
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "./Sql/SqlConnRAII.h"
#include "./Http/Httpconn.h"

#define READ_BUFFER 1024
class Connection;
class Acceptor;
class Server{
public:
    Server(std::shared_ptr<EventLoop> loop,uint32_t port);
    ~Server();
    void handleReadEvents(std::shared_ptr<Socket> socket);
    void newConnection(const std::shared_ptr<Socket>& socket);
    void onHttpConnect(std::shared_ptr<Socket> socket);
    static void onReadEvent(std::shared_ptr<Httpconn> &httpconn,std::shared_ptr<Socket> &socket);
    void onWriteEvent(std::shared_ptr<Httpconn> &httpconn);
    void deleteConnection(const std::shared_ptr<Socket>& socket);
private:
    std::shared_ptr<Acceptor> acceptor;

    //Acceptor* acceptor;
    std::unordered_map<std::shared_ptr<Socket>,std::shared_ptr<Connection>> ConnList;
    std::unordered_map<std::shared_ptr<Socket>,std::shared_ptr<Httpconn>> httpConnList;
    std::shared_ptr<EventLoop> mainReactor;
    std::vector<std::shared_ptr<EventLoop>> subReactor;
    std::shared_ptr<ThreadPool> threadPool;

};
#endif //TCPSERVER_SERVER_H
