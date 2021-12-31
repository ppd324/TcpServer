//
// Created by 裴沛东 on 2021/11/4.
//

#ifndef LOG_MICROTIMESTAMP_H
#define LOG_MICROTIMESTAMP_H


#include <stdint.h>
#include <string>
#include <algorithm>

class MicroTimestamp {
public:
    MicroTimestamp():m_microsecondsSinceEpoch(0){}
    ~MicroTimestamp(){

    }
    explicit MicroTimestamp(int64_t MicrosecondsSinceEpoch):m_microsecondsSinceEpoch(MicrosecondsSinceEpoch) {

    }
    void Swap(MicroTimestamp& other) {
        std::swap(m_microsecondsSinceEpoch,other.m_microsecondsSinceEpoch);
    }

    std::string Tostring() const;

    std::string ToFormattedString(bool showMicroseconds = true) const;

    bool valid() const
    {
        return m_microsecondsSinceEpoch > 0;
    }

    int64_t GetMicroseconds() const
    {
        return m_microsecondsSinceEpoch;
    }

    time_t SecondsSinceEpoch() const
    {
        return static_cast<time_t>(m_microsecondsSinceEpoch / kMicroSecondsPerSecond);
    }

    static MicroTimestamp Now();
    static MicroTimestamp Invalid()
    {
        return MicroTimestamp();
    }

    static MicroTimestamp FromUnixTime(time_t t)
    {
        return FromUnixTime(t, 0);
    }

    static MicroTimestamp FromUnixTime(time_t t, int Microseconds)
    {
        return MicroTimestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + Microseconds);
    }
public:
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t m_microsecondsSinceEpoch; //微秒时间戳

};


#endif //LOG_MICROTIMESTAMP_H
