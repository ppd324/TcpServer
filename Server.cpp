//
// Created by 裴沛东 on 2021/12/28.
//

#include "Server.h"
class Socket;
class InetAddress;
class Channel;
Server::Server(std::shared_ptr<EventLoop> loop, uint32_t port):loop(loop),acceptor(nullptr){
    this->acceptor = std::make_shared<Acceptor>(loop,port);
    std::function<void(std::shared_ptr<Socket>)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}
void Server::newConnection(std::shared_ptr<Socket> socket) {
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(loop,socket);
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
            printf("message from client fd %d: %s\n", socket->get_fd(), buf);
            write(socket->get_fd(), buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", socket->get_fd());
            close(socket->get_fd());   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }

}
Server::~Server() {

}
void Server::deleteConnection(const std::shared_ptr<Socket>& deletesocket) {
    printf("delete client connection fd is %d\n",deletesocket->get_fd());
    ConnList.erase(deletesocket);

}
