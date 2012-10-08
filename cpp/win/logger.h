#pragma once
#ifndef LOGGER_H_
#define LOGGER_H_

#include "logformat.h"
#include "dbg.h"
#include <fstream>

#define LOGGER
#ifdef LOGGER
#define LOGGER_LOG(logger, level, logstr)                             \
    {                                                                 \
        if (logger.logLevel() >= level) {                             \
            logger.log(logstr, level, __FILE__, __LINE__, __func__);  \
        }                                                             \
    }
#define LOGGER_ERROR(logging, logstr)  LOGGER_LOG(logging, LogLevel::ERROR, logstr)
#define LOGGER_WARN(logging, logstr)   LOGGER_LOG(logging, LogLevel::WARN, logstr)
#define LOGGER_INFO(logging, logstr)   LOGGER_LOG(logging, LogLevel::INFO, logstr)
#else
#define LOGGER_LOG(logging, level, logstr)
#define LOGGER_ERROR(logging, logstr)
#define LOGGER_WARN(logging, logstr)
#define LOGGER_INFO(logging, logstr)
#endif

namespace ktn {

class Logger {
public:
    static DefaultLogFormat defaultLogFormat;

    Logger() :
        logLevel_(LogLevel::INFO),
        logFormat_(&defaultLogFormat),
        filename_("log.txt") {}
    explicit Logger(ILogFormat* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_("log.txt") {}
    ~Logger() {}

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const String& log, LogLevel::Level level,
            const char* file, int line, const char* func) {
        String str = logFormat_->format(log, level, file, line, func);
        std::cout << str << std::endl;

        std::ofstream ofs(filename_, std::ios::app);
        ofs << str << std::endl;
    }

private:
    LogLevel::Level logLevel_;
    ILogFormat* logFormat_;
    char* filename_;
};

DefaultLogFormat Logger::defaultLogFormat;

} // namespace ktn

#endif // LOGGER_H_