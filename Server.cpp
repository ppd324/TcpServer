//
// Created by 裴沛东 on 2021/12/28.
//

#include "Server.h"
class Socket;
class InetAddress;
class Channel;
class Httpconn;
Server::Server(std::shared_ptr<EventLoop> loop, uint32_t port):mainReactor(loop),acceptor(nullptr){
    this->acceptor = std::make_shared<Acceptor>(loop,port);
    std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::onHttpConnect, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    //int threadSize = std::thread::hardware_concurrency();
    int threadSize = 5;
    this->threadPool = std::make_shared<ThreadPool>(threadSize);
    for(int i=0;i<threadSize;++i) {
        subReactor.emplace_back(std::make_shared<EventLoop>());
    }

    for(int i=0;i<threadSize;++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop,subReactor[i]);
        threadPool->addTask(sub_loop);
    }
}
void Server::newConnection(const std::shared_ptr<Socket>& socket) {
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(mainReactor,socket);
    std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::deleteConnection,this,socket);
    conn->setDeleteConnetCallback(cb);
    ConnList[socket] = conn;
}

void Server::handleReadEvents(std::shared_ptr<Socket> socket) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(socket->get_fd(), buf, sizeof(buf));
        if(bytes_read > 0){
            LOG_INFO<<"message from client fd is "<<socket->get_fd()<<" content is "<<buf;
            write(socket->get_fd(), buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            LOG_INFO<<"continue reading";
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            LOG_INFO<<"finish reading once";
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            LOG_INFO<<"EOF, client fd "<< socket->get_fd() <<" disconnected";
            close(socket->get_fd());   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }

}
Server::~Server() {

}
void Server::deleteConnection(const std::shared_ptr<Socket>& deletesocket) {
    LOG_INFO<<"delete client connection fd is "<<deletesocket->get_fd();
    if(httpConnList.count(deletesocket)) {
        httpConnList.find(deletesocket)->second->channel->enableDeleting(); //从树上取下
    }
    httpConnList.erase(deletesocket);
    close(deletesocket->get_fd());

}

void Server::onHttpConnect(shared_ptr<Socket> socket) {
    if(socket->get_fd() != -1) {
        int random = socket->get_fd()%subReactor.size();
        std::shared_ptr<Httpconn> conn = std::make_shared<Httpconn>(subReactor[random],socket);
        onReadEvent(conn,socket);
        std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::deleteConnection,this,socket);
        conn->setDeleteConnetCallback(cb);
        httpConnList[socket] = conn;
    }

}

void Server::onReadEvent(shared_ptr<Httpconn> &httpconn,shared_ptr<Socket> &_socket) {
    std::function<void()> cb = std::bind(&Httpconn::handleEvent,httpconn,_socket);
    httpconn->channel->setCallback(cb);
    httpconn->channel->enableReading();
}

void Server::onWriteEvent(shared_ptr<Httpconn> &httpconn) {


}
