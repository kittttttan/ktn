#pragma once
#ifndef LOGGERW_H_
#define LOGGERW_H_

#include "ktn/dbg.h"
#include "ktn/logformatw.h"

#define LOGGERW
#ifdef LOGGERW
#ifndef _T
#define __T(c) L ## c
#define _T(c) __T(c)
#endif
#define LOGGERW_LOG(logging, level, logstr)                                 \
    {                                                                        \
        if (logging.logLevel() >= level) {                                   \
            logging.log(logstr, level, __FILE__, __LINE__, _T(__func__));    \
        }                                                                    \
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
        filename_("log.txt") {}
    explicit LoggerW(ILogFormatW* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_("log.txt") {}
    ~LoggerW() {}

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const StringW& log, LogLevel::Level level,
            const char* file, int line, const wchar_t* func) {
        StringW str = logFormat_->format(log, level, file, line, func);
        str.out();

        FILE* fp = fopen(filename_, "a");
        if (!fp) { return; }
        fwprintf(fp, L"%ls\n", str.string());
        fclose(fp);
    }

private:
    LogLevel::Level logLevel_;
    ILogFormatW* logFormat_;
    char* filename_;
};

DefaultLogFormatW LoggerW::defaultLogFormat;

} // namespace ktn

#endif // LOGGERW_H_