//
// Created by 裴沛东 on 2021/12/29.
//

#include "Buffer.h"
Buffer::Buffer() {

}
Buffer::~Buffer() {

}
void Buffer::append(const char *data, size_t len) {
    for(int i=0;i<len;++i) {
        if(data[i] == '\0') break;
        Buf.push_back(data[i]);
    }

}
void Buffer::clear() {

    Buf.clear();
}
const char * Buffer::c_str() {
    return Buf.c_str();
}
void Buffer::getline() {
    Buf.clear();
    std::getline(std::cin, Buf);
}
size_t Buffer::size() {
    return Buf.size();
}