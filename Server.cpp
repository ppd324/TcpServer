//
// Created by 裴沛东 on 2021/12/28.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "Server.h"

class Socket;
class InetAddress;
class Channel;
class Httpconn;
Server::Server(std::shared_ptr<EventLoop> loop, uint32_t port):mainReactor(loop),acceptor(nullptr){
    this->acceptor = std::make_shared<Acceptor>(loop,port);
    std::function<void(std::shared_ptr<Socket>&)> cb = std::bind(&Server::onHttpConnect, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    //int threadSize = std::thread::hardware_concurrency();
    int threadSize = 5;
    this->threadPool = std::make_shared<ThreadPool>(threadSize);
    SqlConnPool::Instance()->Init("127.0.0.1",3306,"root","root","serverUser",2);
    for(int i=0;i<threadSize;++i) {
        subReactor.emplace_back(std::make_shared<EventLoop>(true));
    }

    for(int i=0;i<threadSize;++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop,subReactor[i]);
        threadPool->addTask(sub_loop);
    }
}
void Server::newConnection(const std::shared_ptr<Socket>& socket) {
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(mainReactor,socket);
    std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::deleteSocket,this,socket);
    conn->setDeleteConnetCallback(cb);
    ConnList[socket] = conn;
}

void Server::handleReadEvents(std::shared_ptr<Socket> &socket) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(socket->get_fd(), buf, sizeof(buf));
        if(bytes_read > 0){
            LOG_INFO("message from client fd is %d content is %s",socket->get_fd(),buf);
            write(socket->get_fd(), buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            LOG_INFO("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            LOG_INFO("finish reading once");
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            LOG_INFO("EOF, client fd %d is disconnected",socket->get_fd());
            close(socket->get_fd());   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }

}
Server::~Server() {

}
void Server::deleteConnection(std::shared_ptr<Socket>& socket) {
    if(httpConnList.count(socket)) {
        LOG_INFO("delete client connection fd is %d",socket->get_fd());
        httpConnList.find(socket)->second->channel->enableDeleting(); //从树上取下
        std::cout<<"Httpconn count is"<<httpConnList[socket].use_count()<<std::endl;
        httpConnList.find(socket)->second->loop->timer->deleteConn(httpConnList.find(socket)->second);
        std::cout<<"Httpconn count is"<<httpConnList[socket].use_count()<<std::endl;
        httpConnList.erase(socket);
        if(socket->get_fd() != -1) {
            close(socket->get_fd());
            socket->set_fd(-1);
        }
    }


}

void Server::onHttpConnect(std::shared_ptr<Socket> &socket) {
    if(socket->get_fd() != -1) {
        int random = socket->get_fd()%subReactor.size();
        std::shared_ptr<Httpconn> conn = std::make_shared<Httpconn>(subReactor[random],socket);
        conn->loop->timer->add(conn,std::bind(&Server::deleteConnection,this,socket));
        onReadEvent(conn,socket);
        std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::deleteConnection,this,socket);
        conn->setDeleteConnetCallback(cb);
        httpConnList[socket] = conn;
    }

}

void Server::onReadEvent(std::shared_ptr<Httpconn> &httpconn,std::shared_ptr<Socket> &_socket) {
    httpconn->loop->timer->update(httpconn);
    //std::bind 绑定智能指针会导致增加引用计数，导致不能正常析构
    std::function<void()> cb = std::bind(&Httpconn::handleEvent,httpconn.get(),_socket);
    httpconn->channel->setCallback(cb);
    httpconn->channel->enableReading();
}

void Server::onWriteEvent(std::shared_ptr<Httpconn> &httpconn) {


}

void Server::deleteSocket(std::shared_ptr<Socket> &socket) {
    if(ConnList.count(socket)) {
        LOG_INFO("delete client connection fd is %d",socket->get_fd());
        ConnList.find(socket)->second->channel->enableDeleting(); //从树上取下
        ConnList.erase(socket);
        if(socket->get_fd() != -1) {
            close(socket->get_fd());
            socket->set_fd(-1);
        }
    }


}
