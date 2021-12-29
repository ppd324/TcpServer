//
// Created by 裴沛东 on 2021/12/29.
//

#include "Connection.h"

#include <utility>
#define READ_BUFFER 1024
Connection::Connection(std::shared_ptr<EventLoop>  _loop, const std::shared_ptr<Socket>& _socket):loop(std::move(_loop)),socket(_socket),channel(nullptr),readBuffer(new Buffer),writeBuffer(new Buffer){
    channel = std::make_shared<Channel>(loop,socket);
    std::function<void()> cb = std::bind(&Connection::echo,this,_socket);
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection() {

}

void Connection::echo(const std::shared_ptr<Socket>& client_socket) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(client_socket->get_fd(), buf, sizeof(buf));
        if(bytes_read > 0){
            readBuffer->append(buf,bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("message from client fd %d: %s\n", client_socket->get_fd(), readBuffer->c_str());
            printf("finish reading once, errno: %d\n", errno);
            errif(write(client_socket->get_fd(), readBuffer->c_str(), readBuffer->size())== -1,"message write failed");
            readBuffer->clear();
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", client_socket->get_fd());
            //close(socket->get_fd());   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnetCallback(client_socket);
            break;
        }
    }

}

void Connection::setDeleteConnetCallback(std::function<void(std::shared_ptr<Socket>)> func) {
    deleteConnetCallback = std::move(func);

}