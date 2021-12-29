//
// Created by 裴沛东 on 2021/12/28.
//

#include "Acceptor.h"

Acceptor::Acceptor( std::shared_ptr<EventLoop>& loop,int port):_loop(loop),_sock(nullptr),_channel(nullptr) {
    _sock = std::make_shared<Socket>();
    InetAddress* addr = new InetAddress("",port);
    _sock->bind(addr);
    _sock->listen();
    _sock->setnonblocking();
    _channel = std::make_shared<Channel>(loop,_sock);
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection,this);
    _channel->setCallback(cb);
    _channel->enableReading();
    delete addr;
}
Acceptor::~Acceptor() {

}
void Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    std::shared_ptr<Socket>clnt_sock = std::make_shared<Socket>(_sock->accept(clnt_addr).get_fd());
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->get_fd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;

}
void Acceptor::setNewConnectionCallback(std::function<void(std::shared_ptr<Socket>)> callback) {
    this->newConnectionCallback = std::move(callback);

}