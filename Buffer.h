//
// Created by 裴沛东 on 2021/12/29.
//

#ifndef TCPSERVER_BUFFER_H
#define TCPSERVER_BUFFER_H

#include <string>

class Buffer {
public:
    Buffer();
    ~Buffer();
    void append(const char* data,size_t len);
    void append(const std::string& data);
    size_t size();
    void init();
    const char* c_str();
    void clear();
    std::string::iterator peek();
    std::string getline();
    void Retrieve(size_t len);
    void RetrieveUntil(std::string::iterator pos);
    bool readable();
    void print();
    size_t readableBytes();

private:
    std::string Buf;
    std::string::iterator readPos_;



};


#endif //TCPSERVER_BUFFER_H
