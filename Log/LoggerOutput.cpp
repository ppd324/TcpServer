//
// Created by 裴沛东 on 2021/11/5.
//

#include "LoggerOutput.h"

LoggerOutput::LoggerOutput(const std::string& filePath, const std::string& fileName, std::size_t logSaveDays, std::size_t fileSize, std::size_t outputmode):
                    m_logFile(filePath,fileName,logSaveDays,fileSize,outputmode),
                    m_outputMode(OUTPUT_CONSOLE)
{
    getOutputCallback();
}

LoggerOutput::~LoggerOutput() {

}
void LoggerOutput::SetConsoleOption(bool isPrint) {
    if(isPrint) {
        m_outputMode &= OUTPUT_CONSOLE;
        getOutputCallback();
    }
}

void LoggerOutput::WriteLog(const std::string& strLog)
{
    m_pOutputCallback(strLog);
}

void LoggerOutput::getOutputCallback() {
    switch(m_outputMode) {
        case OUTPUT_CONSOLE:
            m_pOutputCallback = std::bind(&LoggerOutput::writeToC,this,std::placeholders::_1);
            break;
        case OUTPUT_FILE:
            m_pOutputCallback = std::bind(&LoggerOutput::writeToF,this,std::placeholders::_1);
            break;
        case OUTPUT_CF:
            m_pOutputCallback = std::bind(&LoggerOutput::writeToCF,this,std::placeholders::_1);
            break;
    }

}

void LoggerOutput::writeToC(const std::string &strLog) {
    fwrite(strLog.c_str(),1,strLog.length(),stdout);
}

void LoggerOutput::writeToF(const std::string& strLog)
{
    m_logFile.append(strLog);
}

void LoggerOutput::writeToCF(const std::string& strLog)
{
    fwrite(strLog.c_str(), 1, strLog.length(), stdout);
    m_logFile.asyncAppend(strLog);
}
void LoggerOutput::Flush()
{

}

bool LoggerOutput::setLogMode(size_t mode) {
    m_outputMode = mode;
    getOutputCallback();
    return true;
}

bool LoggerOutput::setFileOption(const std::string& filePath, const std::string& fileName, std::size_t logSaveDays,
                                 std::size_t fileSize) {
    m_logFile.setLogFileOption(filePath, fileName, logSaveDays, fileSize);
    return true;
}
