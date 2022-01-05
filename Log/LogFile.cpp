//
// Created by 裴沛东 on 2021/11/5.
//

#include <ctime>
#include <vector>
#include <functional>
#include "LogFile.h"
size_t LogFile::currentSize = 0;
size_t LogFile::logLines = 0;
size_t LogFile::fileNum = 0;


LogFile::LogFile(const std::string& filepath, const std::string& filename, std::size_t LogSaveDays, std::size_t LogFileSize,std::size_t outputmode):filepath(filepath),filename(filename),LogSaveDays(LogSaveDays),LogFileSize(LogFileSize),m_currentBuffer(nullptr),m_nextBuffer(nullptr) {
    if((outputmode >> 1) % 2 == 1) {
        fileName = filepath+filename+"_log"+std::to_string(fileNum)+".txt";
        std::cout<<"file name is "<<fileName<<std::endl;
        out = std::ofstream(fileName,std::ios_base::in|std::ios_base::trunc);
        fileStoreDays[time(0)] = fileName;
        running = true;
        m_thread =  std::thread(std::mem_fn(&LogFile::threadfunc),this);
    }


}
LogFile::LogFile() {

}

LogFile::~LogFile() {
    running = false;
    if(m_thread.joinable()) {
        m_thread.join();
    }
    if(out.is_open()) {
        out.close();
    }
}
bool LogFile::append(const std::string &strLog) {
    if(out.is_open()) {
        out<<strLog;
        logLines++;
        if(logLines > 1000) {
            currentSize = getFileSize();
            logLines = 0;
            if(currentSize > this->LogFileSize) {
                ++fileNum;
                fileName = filepath+filename+"_log"+std::to_string(fileNum)+".txt";
                if(out.is_open()) {
                    out.close();
                }
                out = std::ofstream(fileName,std::ios_base::in|std::ios_base::trunc);
                fileStoreDays[time(0)] = fileName;
            }
        }
    }else {
        return false;
    }
    return true;
}
bool LogFile::asyncAppend(const std::string &strLog) {
    auto len = strLog.length();
    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_currentBuffer->avail() > len) {
        m_currentBuffer->append(strLog.c_str(),len);
    }else {
        m_bufferVector.emplace_back(std::move(m_currentBuffer));
        if(m_nextBuffer) {
            m_currentBuffer = std::move(m_nextBuffer);
        }else {
            m_currentBuffer.reset(new Buffer);
        }
        m_currentBuffer->append(strLog.c_str(),len);
        m_cond.notify_one();
    }
}

void LogFile::bufferToWriteFile(BufferVector &buffervector) {
    for(auto &buffer : buffervector) {
        out<<buffer->data();
    }
    currentSize = getFileSize();
    if(currentSize > this->LogFileSize) {
        ++fileNum;
        fileName = filepath + filename + "_log" + std::to_string(fileNum) + ".txt";
        if (out.is_open()) {
            out.close();
        }
        out = std::ofstream(fileName, std::ios_base::in | std::ios_base::trunc);
        fileStoreDays[time(0)] = fileName;
    }
    buffervector.clear();
}
void LogFile::threadfunc() {
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector bufferToWrite;
    bufferToWrite.reserve(16);
    while(running) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            /*if(m_bufferVector.empty()) {
                m_cond.wait_until(lock,now + std::chrono::microseconds(1000),[&](){
                    return !m_bufferVector.empty();
                });
                m_cond.wait_for()
            }*/
            if(m_cond.wait_for(lock,std::chrono::microseconds(1000),[&](){return !m_bufferVector.empty();})) {
                m_bufferVector.push_back(std::move(m_currentBuffer));
                m_currentBuffer = std::move(newBuffer1);
                bufferToWrite.swap(m_bufferVector);
                if(!m_nextBuffer) {
                    m_nextBuffer = std::move(newBuffer2);
                }
            }else {
                //std::cout<<"time out"<<std::endl;
                m_bufferVector.push_back(std::move(m_currentBuffer));
                m_currentBuffer = std::move(newBuffer1);
                bufferToWrite.swap(m_bufferVector);
                if(!m_nextBuffer) {
                    m_nextBuffer = std::move(newBuffer2);
                }
            }

        }
        bufferToWriteFile(bufferToWrite);
        newBuffer1 = std::make_unique<Buffer>();
        newBuffer1->bzero();
        if(newBuffer2 == nullptr) {
            newBuffer2 = std::make_unique<Buffer>();
            newBuffer2->bzero();
        }

    }
}
size_t LogFile::getFileSize() {
    std::ifstream in(fileName.c_str());
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.close();
    return size;
}


void LogFile::setFileName(const std::string &filename) {
    this->filename = filename;
}

void LogFile::setFilePath(const std::string &filePath) {
    this->filepath = filePath;
}

void LogFile::setLogFileSize(size_t size) {
    this->LogFileSize = size;
}


void LogFile::setLogSaveDays(size_t days) {
    this->LogSaveDays = days;
}

void LogFile::setLogFileOption(const std::string &filepath, const std::string &filename, std::size_t LogSaveDays,
                               std::size_t LogFileSize) {
    setFilePath(filepath);
    setFileName(filename);
    setLogSaveDays(LogSaveDays);
    setLogFileSize(LogFileSize);
    fileName = filepath+filename+"_log"+std::to_string(fileNum)+".txt";
    if(out.is_open())
        out.close();
    out = std::ofstream(fileName,std::ios_base::in|std::ios_base::trunc);
    fileStoreDays[time(0)] = fileName;

}

void LogFile::monitorFileTime() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::minutes(24*60));
        time_t now = time(0);
        auto tmp = fileStoreDays.begin();
        while(tmp != fileStoreDays.end()) {
            if((now-tmp->first)/86400 > LogSaveDays) {
                remove(tmp->second.c_str());
                fileStoreDays.erase(tmp++);
            }else {
                tmp++;
            }

        }
    }

}