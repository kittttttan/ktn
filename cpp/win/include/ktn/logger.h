#pragma once

#include "logformat.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <Windows.h>

#ifdef LOGGER_OFF
#define LOGGER_INIT                 ((void)0)
#define LOGGER(name)                ((void)0)
#define LOGGER_LEVEL(level)         ((void)0)
#define LOGGER_LOG(level, logstr)   ((void)0)
#define LOGGER_ERROR(logstr)        ((void)0)
#define LOGGER_WARN(logstr)         ((void)0)
#define LOGGER_INFO(logstr)         ((void)0)
#else
#define LOGGER_INIT  ktn::Logger*_logger=nullptr
#define LOGGER(name) { _logger = ktn::Logger::getInstance(name); }
#define LOGGER_LEVEL(level) { _logger->logLevel(level); }
#define LOGGER_LOG(level, logstr)                                           \
    do {                                                                    \
        if (_logger->logLevel() >= level) {                                 \
            _logger->log(logstr, level, __FILE__, __LINE__, __FUNCTION__);  \
        }                                                                   \
    } while(0)
#define LOGGER_ERROR(logstr)  LOGGER_LOG(ktn::LogLevel::ERR, logstr)
#define LOGGER_WARN(logstr)   LOGGER_LOG(ktn::LogLevel::WARN, logstr)
#define LOGGER_INFO(logstr)   LOGGER_LOG(ktn::LogLevel::INFO, logstr)
#endif

namespace ktn {

class Logger {
public:
    static DefaultLogFormat defaultLogFormat;
    static const DWORD maxWaitTime;

    static Logger* getInstance(const char* name);
    static std::string& name() { return name_; }

    ~Logger();

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const char* log, LogLevel::Level level,
            const char* file, int line, const char* func);

private:
    static HANDLE hWriteEvent_;
    static HANDLE hThread_;
    static std::string name_;

    static void process();

    Logger();
    Logger(const Logger& rhs);
    Logger& operator=(const Logger& rhs);

    void write();

    LogLevel::Level logLevel_;
    ILogFormat* logFormat_;
    char* filename_;
    std::queue<std::string> queue_;
};

} // namespace ktn
