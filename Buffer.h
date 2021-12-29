//
// Created by 裴沛东 on 2021/12/29.
//

#ifndef TCPSERVER_BUFFER_H
#define TCPSERVER_BUFFER_H

#include <string>
#include <iostream>
class Buffer {
public:
    Buffer();
    ~Buffer();
    void append(const char* data,size_t len);
    size_t size();
    const char* c_str();
    void clear();
    void getline();
private:
    std::string Buf;



};


#endif //TCPSERVER_BUFFER_H
