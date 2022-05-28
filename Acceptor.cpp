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
    std::shared_ptr<Socket> client_sock = nullptr;
    while((client_sock = _sock->accept()) != nullptr) {
        client_sock->setnonblocking();
        newConnectionCallback(client_sock);
    }

}
void Acceptor::setNewConnectionCallback(std::function<void(std::shared_ptr<Socket>&)> callback) {
    this->newConnectionCallback = std::move(callback);

}