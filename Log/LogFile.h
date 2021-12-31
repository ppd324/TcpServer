//
// Created by 裴沛东 on 2021/11/5.
//

#ifndef LOG_LOGFILE_H
#define LOG_LOGFILE_H
#include <string>
#include <fstream>
#include <cstdio>
#include <unordered_map>
#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "LogBuffer.h"
class LogFile {
public:
    LogFile(const std::string& filepath,const std::string& filename,std::size_t LogSaveDays,std::size_t LogFileSize,std::size_t outputmode);
    LogFile();
    ~LogFile();
    bool append(const std::string& strLog);
    bool asyncAppend(const std::string& strLog);
    size_t getFileSize();
    void setFileName(const std::string& filename);
    void setFilePath(const std::string& filePath);
    void setLogFileSize(size_t size);
    void setLogSaveDays(size_t days);
    void setLogFileOption(const std::string &filepath,const std::string &filename,std::size_t LogSaveDays,std::size_t LogFileSize);
    void getCurrentDays();
    void monitorFileTime();
    void threadfunc();


private:
    typedef Log::buffer::buffer<Log::buffer::LargeBuffer> Buffer;  //4MB buffer
    typedef std::unique_ptr<Buffer> BufferPtr;                     //缓冲区指针
    typedef std::vector<BufferPtr> BufferVector;                   //
    std::ofstream out;
    std::string filename;
    std::string filepath;
    std::string fileName;
    size_t LogFileSize;
    size_t LogSaveDays;
    static size_t currentSize;
    static size_t logLines;
    static size_t fileNum;
    std::unordered_map<time_t,std::string> fileStoreDays;
    std::atomic<bool> running;
    std::thread m_thread;
    std::mutex  m_mutex;
    std::condition_variable m_cond;
    BufferPtr m_currentBuffer;
    BufferPtr m_nextBuffer;
    BufferVector m_bufferVector;
    void bufferToWriteFile(BufferVector &buffervector);




};


#endif //LOG_LOGFILE_H
