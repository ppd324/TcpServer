#include <iostream>
#include "Server.h"
#include "EventLoop.h"
#include "./Http/HttpRequest.h"
using namespace Log;
int main() {
    initLogLevel();
    Logger::setLogFmtType(Logger::STRING);
    Logger::setLogMode(OUTPUT_CONSOLE);
    auto loop = std::make_shared<EventLoop>();
    auto server = std::make_shared<Server>(loop,8888);
    loop->loop();
//    std::shared_ptr<Buffer> buf = std::make_shared<Buffer>();
//    HttpRequest *httpRequest = new HttpRequest();
//    std::string httpPost = "GET /b82.css HTTP/1.1\r\n"
//                           "Host: 172.18.0.3\r\n"
//                           "Connection: keep-alive\r\n"
//                           "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36 Edg/96.0.1054.62\r\n"
//                           "Accept: text/css,*/*;q=0.1\r\n"
//                           "Referer: http://172.18.0.3/\r\n"
//                           "Accept-Encoding: gzip, deflate\r\n"
//                           "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
//                           "Cookie: vlan=0; program=xupt-20171115; save_DDDDD=dk431; save_upass=dianke; username=dk431; md5_login2=dk431%7Cdianke; ip=222.24.22.123\r\n"
//                           "\r\n"
//                           "hello world,sfdnjsgksjkdgjksg\r\n";
//    buf->append(httpPost.c_str(),httpPost.size());
//    std::cout<<buf->c_str()<<std::endl;
//    //buf->print();
//    httpRequest->parse(buf);
//    httpRequest->printHead();
//    std::cout<<httpRequest->path()<<std::endl;
//    std::cout<<httpRequest->version()<<std::endl;
//    std::cout<<httpRequest->method()<<std::endl;
//    //std::cout<<buf->getline()<<std::endl;
//    std::cout<<httpRequest->body()<<std::endl;
//    delete httpRequest;
    //std::cout<<buf->getline()<<std::endl;
    return 0;
}
