//
// Created by 裴沛东 on 2021/12/28.
//

#include "Acceptor.h"

Acceptor::Acceptor( std::shared_ptr<EventLoop>& loop,int port):_loop(loop),_sock(nullptr),_channel(nullptr) {
    _sock = std::make_shared<Socket>("",port);
    _sock->setnonblocking();
    _sock->setsockopt();
    _channel = std::make_shared<Channel>(loop,_sock);
    _channel->setNotUseThreadPool(true);
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection,this);
    _channel->setCallback(cb);
    _channel->enableListening();

}
Acceptor::~Acceptor() {

}
void Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    std::shared_ptr<Socket>clnt_sock = std::make_shared<Socket>(_sock->accept(clnt_addr).get_fd());
    LOG_INFO("new client fd is %d, ip is %s ,port is %d",clnt_sock->get_fd(),inet_ntoa(clnt_addr->addr.sin_addr),ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);

}
void Acceptor::setNewConnectionCallback(std::function<void(std::shared_ptr<Socket>)> callback) {
    this->newConnectionCallback = std::move(callback);

}