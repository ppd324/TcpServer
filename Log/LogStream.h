//
// Created by 裴沛东 on 2021/11/4.
//

#ifndef LOG_LOGSTREAM_H
#define LOG_LOGSTREAM_H
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;
class LogStream{
public:
    LogStream();
    ~LogStream();

public:
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(thread::id);
    LogStream& operator<<(float v){
        *this<<static_cast<double >(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(char v) {
        m_stream<<v;
        return *this;
    }
    LogStream& operator<<(unsigned char v) {
        m_stream<<v;
        return *this;
    }
    LogStream& operator<<(const char* str) {
        if(str) {
            m_stream<<str;
        }else {
            m_stream<<"(null)";
        }
        return *this;
    }
    LogStream& operator<<(const unsigned char* str) {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const string& str) {
        m_stream << str;
        return *this;
    }

    const stringstream& GetStream() const { return m_stream; }
    const string toString() const { return m_stream.str();}
    void clearStream() { m_stream.clear(); }


private:
    void staticCheck();
    stringstream m_stream;
    static const int kMaxNumericSize = 32;

};
#endif //LOG_LOGSTREAM_H
