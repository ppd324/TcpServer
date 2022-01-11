//
// Created by ppd on 2022/1/9.
//

#ifndef TCPSERVER_LOG_H
#define TCPSERVER_LOG_H
#include <mutex>
#include <memory>
#include <string>
#include <thread>
#include <sys/stat.h>
#include <iomanip>
#include <stdarg.h>
#include <sstream>
#include <sys/time.h>
#include "BlockingQueue.h"

class Log {
public:
    static Log* Instance();
    void init(int level, const char* path = "./log",
              const char* suffix =".log",
              size_t maxQueueCapacity = 1024);
    static void FlushLogThread();
    void write(const char *filename, int line, const char *func,int level, const char *format,...);
    void flush();
    int GetLevel();
    void SetLevel(int level);
    bool IsOpen() { return _isOpen; }
private:
    Log();
    void AppendLogLevelTitle_(int level);
    virtual ~Log();
    void AsyncWrite_();
private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;
    const char* path_;
    const char* suffix_;
    std::unique_ptr<BlockingQueue<std::string>> _Que;
    std::unique_ptr<std::thread> _WriterThread;
    bool _isOpen;
    bool _isAsync;
    int _outPutLevel;
    FILE* _fp;
    long _lineCount;
    std::mutex _mutex;
    int _Today;
    std::stringstream _logLine;

};
#define LOG_BASE(level, format, ...) \
 do{                                 \
     Log *log = Log::Instance();     \
     if(log->IsOpen() && log->GetLevel() <= level) { \
        log->write(__FILE__,__LINE__,__FUNCTION__,level,format, ##__VA_ARGS__);   \
        log->flush(); \
 }                                   \
 }while(0);

#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);
#endif //TCPSERVER_LOG_H
