//
// Created by 裴沛东 on 2021/11/5.
//

#ifndef LOG_LOGGEROUTPUT_H
#define LOG_LOGGEROUTPUT_H
#include <string>
#include <cstdio>
#include <functional>
#include "LogFile.h"
const std::size_t OUTPUT_CONSOLE	  = 0x0001; //只输出到控制台console
const std::size_t OUTPUT_FILE	  = 0x0002; //只输出到日志文件
const std::size_t OUTPUT_NETTCP   = 0x0004; //只输出到TCP网络
const std::size_t OUTPUT_NETHTTP   = 0x0008; //只输出到HTTP网络
const std::size_t OUTPUT_CF = OUTPUT_CONSOLE & OUTPUT_FILE;
const std::size_t OUTPUT_CNT = OUTPUT_CONSOLE & OUTPUT_NETTCP;
const std::size_t OUTPUT_CNH  = 0x0009; //输出到控制台和HTTP网络
const std::size_t OUTPUT_FNT  = 0x0006;	//输出到日志文件、TCP网络
const std::size_t OUTPUT_FNH  = 0x000A;	//输出到日志文件、HTTP网络
const std::size_t OUTPUT_CFNT = 0x0007; //输出到控台、日志文件、TCP网络
const std::size_t OUTPUT_CFNH = 0x000C; //输出到控台、日志文件、HTTP网络
const static std::size_t kLogSaveDays = 7;
const static std::size_t kLogFileSize = 1024*1024*100;
const static std::string kLogBaseName = "test";
static std::size_t DEFAULT_OUT_PUT_MOD = OUTPUT_CONSOLE;
#ifdef _WIN32
const static std::string kLogBasePath = "H:\\c++\\Log\\";
#else
const static std::string kLogBasePath = "./log/";
#endif
typedef std::function<void(const std::string&)> pOutPutFunc;
class LoggerOutput {
public:
    LoggerOutput(const std::string& filePath = kLogBasePath,
                 const std::string& fileName = kLogBaseName,
                 std::size_t logSaveDays = kLogSaveDays,
                 std::size_t fileSize = kLogFileSize,
                 std::size_t outputmode = DEFAULT_OUT_PUT_MOD);
    ~LoggerOutput();

    bool setFileOption(const std::string& filePath = kLogBasePath,
                       const std::string& fileName = kLogBaseName,
                       std::size_t logSaveDays = kLogSaveDays,
                       std::size_t fileSize = kLogFileSize);

    bool setNetOfTcpOption(std::string destIP,uint32_t destPort);

    bool SetNetOfHttpOption(std::string url);

    void SetConsoleOption(bool isPrint);

    bool SetLogFileFlushOption(const size_t nFlushInterval, const size_t checkEveryN);

    bool setLogMode(size_t mode);

    void WriteLog(const std::string& strLog);

    void Flush();


private:
    void getOutputCallback();
    void writeToC(const std::string& strLog);
    void writeToF(const std::string& strLog);
    void writeToNT(const std::string& strLog);
    void writeToNH(const std::string& strLog);
    void writeToCF(const std::string& strLog);
    void writeToCNT(const std::string& strLog);
    void writeToCNH(const std::string& strLog);
    void writeToFNT(const std::string& strLog);
    void writeToFNH(const std::string& strLog);
    void writeToCFNT(const std::string& strLog);
    void writeToCFNH(const std::string& strLog);
private:
    std::size_t m_outputMode;
    pOutPutFunc m_pOutputCallback;
    LogFile m_logFile;

};


#endif //LOG_LOGGEROUTPUT_H
