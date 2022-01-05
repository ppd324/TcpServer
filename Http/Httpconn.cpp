//
// Created by 裴沛东 on 2021/12/31.
//

#include "Httpconn.h"

#include <utility>
#include "../Server.h"
const char* Httpconn::srcDir = "../resources";
std::atomic<int> Httpconn::userCount;
bool Httpconn::isET = true;
Httpconn::Httpconn(std::shared_ptr<EventLoop> _loop, const std::shared_ptr<Socket> &_socket): Connection(std::move(_loop),_socket),httpRequest_() {


}
size_t Httpconn::write(std::shared_ptr<Socket>& _socket) {
    ssize_t len = -1;
    do {
        len = writev(_socket->get_fd(),iov_,iovCnt_);
        if(len < 0) {
            deleteConnetCallback(_socket);
            break;
        }
        if(iov_[0].iov_len + iov_[1].iov_len == 0) break;
        else if(static_cast<size_t>(len) > iov_[0].iov_len) {
            iov_[1].iov_base = (uint8_t*) iov_[1].iov_base + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if(iov_[0].iov_len) {
                writeBuffer->clear();
                iov_[0].iov_len = 0;
            }
        }else {
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len;
            iov_[0].iov_len -= len;
            writeBuffer->Retrieve(len);
        }

    } while (isET || ToWriteBytes() > 10240);
    return len;

}
size_t Httpconn::read(std::shared_ptr<Socket>& _socket) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = ::read(_socket->get_fd(), buf, sizeof(buf));
        if(bytes_read > 0){
            LOG_DEBUG<<"read "<<bytes_read<<" bytes";
            readBuffer->append(buf,bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            deleteConnetCallback(_socket);
            break;
        }
    }

    return 0;

}
bool Httpconn::process() {
    if(!readBuffer->readable()) return false;
    if(httpRequest_.parse(readBuffer)) {
        LOG_DEBUG<<httpRequest_.path();
        LOG_DEBUG<<"http connectstatus is"<<httpRequest_.IsKeepAlive();
        httpResponse_.Init(srcDir, httpRequest_.path(), httpRequest_.IsKeepAlive(), 200);
    } else {
        httpResponse_.Init(srcDir, httpRequest_.path(), false, 400);
    }
    httpResponse_.makeResponse(writeBuffer);
    LOG_DEBUG<<"response maked";
    iov_[0].iov_base = const_cast<char*>(writeBuffer->c_str());
    iov_[0].iov_len = writeBuffer->readableBytes();
    iovCnt_ = 1;

    if(httpResponse_.FileLen() > 0 && httpResponse_.File()) {
        iov_[1].iov_base = httpResponse_.File();
        iov_[1].iov_len = httpResponse_.FileLen();
        iovCnt_ = 2;
    }
    LOG_DEBUG<<"filesize:"<< httpResponse_.FileLen() <<" iovCnt_: " <<iovCnt_<<" writeBufSize:"<<writeBuffer->readableBytes();
    return true;

}


bool Httpconn::handleEvent(std::shared_ptr<Socket> &_socket) {
    read(_socket);
    if(process()) {
        std::function<void()> cb = std::bind(&Httpconn::write,this,_socket);
        channel->setCallback(cb);
        channel->enableWriting();
        //write(_socket);
    }


}

Httpconn::~Httpconn() {

}
