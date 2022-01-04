//
// Created by 裴沛东 on 2021/12/31.
//

#include "Httpconn.h"

Httpconn::Httpconn(std::shared_ptr<EventLoop> _loop, const std::shared_ptr<Socket> &_socket): Connection(_loop,_socket),httpRequest_() {


}
size_t Httpconn::write() {
    return 0;

}
size_t Httpconn::read() {
    return 0;

}
bool Httpconn::process() {

}