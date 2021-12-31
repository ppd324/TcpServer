//
// Created by 裴沛东 on 2021/11/4.
//
#ifdef _WIN32
#include <Windows.h>
#include <winsock.h>
#include <ctime>

#else
#include <sys/time.h>
#endif

#include "MicroTimestamp.h"
MicroTimestamp MicroTimestamp::Now() {
    struct timeval tv;
#ifdef _WIN32
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tv.tv_sec = clock;
    tv.tv_usec = wtm.wMilliseconds;
#else
    gettimeofday(&tv, NULL);
#endif
    int64_t seconds = tv.tv_sec;
    return std::move(MicroTimestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec));

}
std::string MicroTimestamp::Tostring() const {
    char buf[32] = {0};
    int64_t seconds = m_microsecondsSinceEpoch/kMicroSecondsPerSecond;
    int64_t Microseconds = m_microsecondsSinceEpoch%kMicroSecondsPerSecond;
    snprintf(buf,sizeof(buf)-1,"%lld.%06lld",seconds,Microseconds);
    return buf;
}

std::string MicroTimestamp::ToFormattedString(bool showMicroseconds) const {
    char buf[64] = {0};
    time_t seconds = reinterpret_cast<time_t>(m_microsecondsSinceEpoch/kMicroSecondsPerSecond);
    struct tm tm_time;
#ifdef _WIN32
    gmtime_s(&tm_time,&seconds);
#else
    gmtime_r(&seconds, &tm_time);
#endif
    if(showMicroseconds) {
        int microseconds = static_cast<int>(m_microsecondsSinceEpoch % kMicroSecondsPerSecond);
        snprintf(buf,sizeof(buf)-1,"%d-%02d-%02d %02d:%02d:%02d.%06d",tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,microseconds);

    }else {
        snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}
