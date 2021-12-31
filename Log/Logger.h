//
// Created by 裴沛东 on 2021/11/4.
//

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H
#include "LogStream.h"
#include "MicroTimestamp.h"
#include "LoggerOutput.h"
namespace Log {
    class Logger {
    public:
        enum LogLevel {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,

        };
        enum LogFmtType
        {
            STRING,
            JSON,
        };
        Logger(const char *filename, int line,LogLevel lever, const char *func);
        ~Logger();
        /*********************************************
	    **说明：通过该函数可以获取logger中stream类型
	    **param：
	    **		无
	    **return:
	    **		type ：CLogStream&
	    ********************************************/
        LogStream& stream() { return m_logstream; }
        /****************************************
         * 说明：获取日志等级
         * param:
         *       无
         * return：
         *      type : LogLevel
         *
         */
         static LogLevel logLevel();
        /*********************************************
        **说明：通过该函数可以设置日志输出的级别
        **param：
        **		level：日志的输出级别
        ********************************************/
        static void setLogLevel(Logger::LogLevel level);
        /*********************************************
	    **说明：通过该函数可以设置日志写文件的参数
	    **param：
	    **		strLogPath：设置日志的输出路径，该目录可不存在
	    **		strLogBaseName： 设置日志的基本名称
	    **		logLevel：设置日志的输出级别
	    **		logFileSize： 设置每个日志文件的最大文件大小
	    **		logSaveDays：设置日志的保存天数
	    **		logFmtType：设置日志的输出格式，默认普通字符串
	    ********************************************/
        static void setOutputFileOptions(const std::string&strLogPath,const std::string& strLogBaseName
                ,const LogLevel logLevel,const size_t logFileSize,const size_t logSaveDays = 7,const LogFmtType logFmtType = STRING
        );

        static void setLogFmtType(Logger::LogFmtType type);

        static void setLogMode(size_t mode);

        static void setLogFile(const std::string &filepath, const std::string &filename, std::size_t LogSaveDays,
                               std::size_t LogFileSize);
        /***********************************
         * 说明：拼接日志字符串
         * param：NULL
         *
         *
         */
         void writelog();
         void fmtToJson();


    private:
        MicroTimestamp m_time;
        const char* m_filename;
        LogLevel m_lever;
        bool m_isflush;
        int m_nline;
        LogStream m_logstream;
        const char* m_strFunc;
        stringstream m_head;
    public:
        static LogFmtType	  s_logFmtType;
        static LogLevel		  s_writeLogLevel;
        static LoggerOutput   m_loggerOutput;
        static std::string    s_strLogTimeZone;

    };
#define LOG_TRACE if(Logger::logLevel() >= Logger::TRACE) \
    Logger(__FILE__, __LINE__, Logger::TRACE, __FUNCTION__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::DEBUG, __FUNCTION__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__, Logger::INFO, __FUNCTION__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN,__FUNCTION__).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR,__FUNCTION__).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL,__FUNCTION__).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()
    inline Logger::LogLevel Logger::logLevel() {
        return s_writeLogLevel;
    }
    void initLogLevel();
}

#endif //LOG_LOGGER_H
