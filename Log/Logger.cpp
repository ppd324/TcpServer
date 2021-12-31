//
// Created by 裴沛东 on 2021/11/4.
//

#include <iomanip>
#include "Logger.h"
using namespace Log;
Logger::LogFmtType  Logger::s_logFmtType = Logger::STRING;
Logger::LogLevel		Logger::s_writeLogLevel = Logger::TRACE;
LoggerOutput	  Logger::m_loggerOutput;
std::string    Logger::s_strLogTimeZone = "";
const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
};

Logger::Logger(const char *filename, int line,LogLevel lever, const char *func)
:m_filename(filename), m_nline(line),m_lever(lever),m_strFunc(func),m_time(MicroTimestamp::Now())
{

}
Logger::~Logger() {
    if (!m_isflush)
    {
        writelog();
        m_loggerOutput.WriteLog(m_head.str());
    }
    if (m_lever == FATAL)
    {
        m_loggerOutput.Flush();
        abort();
    }

}
void Logger::setLogLevel(Logger::LogLevel level) {
    s_writeLogLevel = level;

}
void Logger::fmtToJson() {
    m_head <<"{ \"timestamp\":";
    m_head <<"\"" << m_time.ToFormattedString() << "\",";
    m_head << "\"file\":\"" << m_filename << "\",";
    m_head << "\"logLevel\":\"" << LogLevelName[m_lever] << "\",";
    m_head << "\"line\":" << m_nline << ",";
    m_head << "\"func\":\"" << m_strFunc << "\",";
    m_head << "\"content\":\"" << m_logstream.toString() << "\"}";
    m_head << "\n";
}
void Logger::writelog() {
    switch(s_logFmtType) {
        case Logger::STRING:
            m_head<<"["<<m_time.ToFormattedString()<<"] ["<<m_filename<<"] ["<<std::setw(5)<<std::setfill(' ')<<std::left <<
            LogLevelName[m_lever]<<"] ["<<std::setw(5)<<std::setfill(' ') << std::left << m_nline << "] ["
                                                      << m_strFunc << "] " << m_logstream.toString()<<"\n";
            break;
        case Logger::JSON:
            fmtToJson();
            break;
        default:
            break;

    }
}
void Logger::setLogFmtType(Logger::LogFmtType type) {
    Logger::s_logFmtType = type;
}
void Log::initLogLevel() {
    Logger::setLogLevel(Logger::TRACE);
    return;
}

void Logger::setLogMode(size_t mode) {
    m_loggerOutput.setLogMode(mode);
}

void Logger::setLogFile(const std::string &filepath, const std::string &filename, std::size_t LogSaveDays,
                        std::size_t LogFileSize) {
    m_loggerOutput.setFileOption(filepath,filename,LogSaveDays,LogFileSize);
}


