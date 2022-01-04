//
// Created by 裴沛东 on 2021/12/29.
//

#include <cassert>
#include "Buffer.h"
Buffer::Buffer():readPos_(Buf.begin()) {

}
Buffer::~Buffer() {

}
void Buffer::append(const char *data, size_t len) {
    for(int i=0;i<len;++i) {
        if(data[i] == '\0') break;
        Buf.push_back(data[i]);
    }
    readPos_ = Buf.begin();

}
void Buffer::clear() {

    Buf.clear();
}
const char * Buffer::c_str() {
    return Buf.c_str();
}
std::string Buffer::getline() {
    const char CRLF[] = "\r\n";
    auto linend = std::search(peek(),Buf.end(),CRLF,CRLF+2);
    std::string res = std::string(peek(),linend);
    if(linend == Buf.end()) {
        readPos_ = linend;
    }else {
        readPos_ = linend + 2;
    }
    return res;

}
size_t Buffer::size() {
    return Buf.size();
}
std::string::iterator Buffer::peek() {
    return readPos_;
}

void Buffer::Retrieve(size_t len) {
    readPos_ += len;
}

void Buffer::RetrieveUntil(std::string::iterator pos) {
    assert(peek() <= Buf.end());
    Retrieve(pos-peek());

}

bool Buffer::readable() {
    return readPos_ != Buf.end();
}
void Buffer::print() {
    for(auto it = Buf.begin();it != Buf.end();it++) {
        std::cout<<*it;
    }
}
size_t Buffer::readableBytes() {
    return Buf.end() - readPos_;
}