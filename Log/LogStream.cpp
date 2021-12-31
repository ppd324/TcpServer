//
// Created by 裴沛东 on 2021/11/4.
//

#include <limits>
#include "LogStream.h"
LogStream::LogStream() {

}
LogStream::~LogStream() {

}
void LogStream::staticCheck() {
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
                  "kMaxNumericSize is large enough");
}
LogStream & LogStream::operator<<(int val) {
    m_stream<<val;
    return *this;

}
LogStream & LogStream::operator<<(long val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(short val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(double val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(long long val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(unsigned int val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(unsigned long val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(unsigned short val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(unsigned long long val) {
    m_stream<<val;
    return *this;
}
LogStream & LogStream::operator<<(thread::id threadid) {
    m_stream<<threadid;
    return *this;
}