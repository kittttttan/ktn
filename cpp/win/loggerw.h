#pragma once

#include "logformatw.h"
#include "dbg.h"
#include <tchar.h>
#include <codecvt>

#define LOGGERW
#ifdef LOGGERW
#define LOGGERW_LOG(logger, level, logstr)                                  \
    {                                                                       \
        if (logger.logLevel() >= level) {                                   \
            logger.log(logstr, level, _T(__FILE__), __LINE__, _T(__func__));\
        }                                                                   \
    }
#define LOGGERW_ERROR(logging, logstr)  LOGGERW_LOG(logging, LogLevel::ERROR, logstr)
#define LOGGERW_WARN(logging, logstr)   LOGGERW_LOG(logging, LogLevel::WARN, logstr)
#define LOGGERW_INFO(logging, logstr)   LOGGERW_LOG(logging, LogLevel::INFO, logstr)
#else
#define LOGGERW_LOG(logging, level, logstr)
#define LOGGERW_ERROR(logging, logstr)
#define LOGGERW_WARN(logging, logstr)
#define LOGGERW_INFO(logging, logstr)
#endif

namespace ktn {

class LoggerW {
public:
    static DefaultLogFormatW defaultLogFormat;

    LoggerW() :
        logLevel_(LogLevel::INFO),
        logFormat_(&defaultLogFormat),
        filename_(L"ktn.log") {}
    explicit LoggerW(ILogFormatW* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_(L"ktn.log") {}
    ~LoggerW() {}

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const wchar_t* log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func);

private:
    LogLevel::Level logLevel_;
    ILogFormatW* logFormat_;
    wchar_t* filename_;
};

} // namespace ktn
