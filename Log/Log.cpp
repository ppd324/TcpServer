//
// Created by ppd on 2022/1/9.
//

#include "Log.h"

Log::Log():_lineCount(0),_WriterThread(nullptr),_Que(nullptr),_isAsync(false),_fp(nullptr) {

}

Log *Log::Instance() {
    static Log log;
    return &log;

}

void Log::write(const char *filename, int line, const char *func,int level, const char *format,...) {
    struct timeval now = {0,0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list valist;
    char buf[256] = {0};

    if(_Today != t.tm_mday || (_lineCount && (_lineCount % MAX_LINES == 0))) {
        std::unique_lock<std::mutex> locker(_mutex);
        locker.unlock();
        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};
        snprintf(tail,36,"%04d_%02d_%02d",t.tm_year+1900,t.tm_mon+1,t.tm_mday);

        if(_Today != t.tm_mday) {
            snprintf(newFile,LOG_NAME_LEN-72,"%s%s%s",path_,tail,suffix_);
            _lineCount = 0;
        }else {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%ld%s", path_, tail, (_lineCount  / MAX_LINES), suffix_);
        }
        locker.lock();
        flush();
        if(_Today != t.tm_mday) {
            _Today = t.tm_mday;
            fclose(_fp);
            _fp = fopen(newFile,"a");
            assert(_fp != NULL);
        }
    }
    {
        std::unique_lock<std::mutex> locker(_mutex);
        _logLine<<t.tm_year+1900 <<"-"<<t.tm_mon+1<<"-"<<std::setw(2)<<t.tm_mday<<" "<<std::setw(2)<<t.tm_hour<<":"<<std::setw(2)<<
        t.tm_min<<":"<<t.tm_sec<<"."<<std::setw(6)<<std::left<<now.tv_usec;
        _logLine<<"{"<<filename<<"}"<<" [FUNC]:"<<func<<":"<<line;
        AppendLogLevelTitle_(level);
        va_start(valist, format);
        vsnprintf(buf, 256, format, valist);
        va_end(valist);
        _logLine << buf <<"\n\0";
        if(_isAsync && _Que && !_Que->full()) {
            _Que->push(_logLine.str());
        } else {
            fputs(_logLine.str().c_str(),_fp);
        }
        _logLine.clear();
        _logLine.str("");
    }

}

void Log::init(int level, const char *path, const char *suffix, size_t maxQueueCapacity) {
    _isOpen = true;
    _outPutLevel = level;
    if(maxQueueCapacity > 0) {
        _isAsync = true;
        _Que = std::make_unique<BlockingQueue<std::string>>(maxQueueCapacity);
        _WriterThread = std::make_unique<std::thread>(FlushLogThread);
    }else {
        _isAsync = false;
    }
    _lineCount = 0;
    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    path_ = path;
    suffix_ = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    snprintf(fileName,LOG_NAME_LEN-1,"%s/%04d_%02d_%02d%s",path_,t.tm_year+1900,t.tm_mon+1,t.tm_mday,suffix_);
    _Today = t.tm_mday;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _logLine.clear();
        if(_fp) {
            flush();
            fclose(_fp);
        }

        _fp = fopen(fileName,"a");
        if(_fp == nullptr) {
            mkdir(path_,0777);
            _fp = fopen(fileName,"a");
        }
        assert(_fp != nullptr);
    }



}

void Log::AppendLogLevelTitle_(int level) {
    switch(level) {
        case 0:
            _logLine<<"[debug]: ";
            break;
        case 1:
            _logLine<<"[info] : ";
            break;
        case 2:
            _logLine<<"[warn] : ";
            break;
        case 3:
            _logLine<<"[error]: ";
            break;
        default:
            _logLine<<"[info] : ";
            break;
    }
}

void Log::FlushLogThread() {
    Log::Instance()->AsyncWrite_();
}

void Log::SetLevel(int level) {
    std::lock_guard<std::mutex> lock(_mutex);
    this->_outPutLevel = level;

}

int Log::GetLevel() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _outPutLevel;
}
void Log::AsyncWrite_() {
    std::string str = "";
    while(_Que->pop(str)) {
        std::lock_guard<std::mutex> locker(_mutex);
        fputs(str.c_str(), _fp);
        fwrite(str.c_str(),1,str.length(),stdout);
    }

}

void Log::flush() {
    if(_isAsync) {
        _Que->flush();
    }
    fflush(_fp);
}
Log::~Log() {
    if(_WriterThread && _WriterThread->joinable()) {
        while(!_Que->empty()) {
            _Que->flush();
        };
        _Que->close();
        _WriterThread->join();
    }
    if(_fp) {
        std::lock_guard<std::mutex> locker(_mutex);
        flush();
        fclose(_fp);
    }
}