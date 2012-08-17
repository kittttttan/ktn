#pragma once
#ifndef LOGGINGW_H_
#define LOGGINGW_H_

#include "logformatw.h"
#include "dbg.h"

#define LOGGINGW
#ifdef LOGGINGW
#ifndef _T
#define __T(c) L ## c
#define _T(c) __T(c)
#endif
#define LOGGINGW_LOG(logging, level, logstr)                                 \
    {                                                                        \
        if (logging.logLevel() >= level) {                                   \
            logging.log(logstr, level, __FILE__, __LINE__, _T(__func__));    \
        }                                                                    \
    }
#define LOGGINGW_ERROR(logging, logstr)  LOGGINGW_LOG(logging, LogLevel::ERROR, logstr)
#define LOGGINGW_WARN(logging, logstr)   LOGGINGW_LOG(logging, LogLevel::WARN, logstr)
#define LOGGINGW_INFO(logging, logstr)   LOGGINGW_LOG(logging, LogLevel::INFO, logstr)
#else
#define LOGGINGW_LOG(logging, level, logstr)
#define LOGGINGW_ERROR(logging, logstr)
#define LOGGINGW_WARN(logging, logstr)
#define LOGGINGW_INFO(logging, logstr)
#endif

namespace ktn {

class LoggingW {
public:
    static DefaultLogFormatW defaultLogFormat;

    LoggingW() :
        logLevel_(LogLevel::INFO),
        logFormat_(&defaultLogFormat),
        filename_("log.txt") {}
    explicit LoggingW(ILogFormatW* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_("log.txt") {}
    ~LoggingW() {}

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

DefaultLogFormatW LoggingW::defaultLogFormat;

} // namespace ktn

#endif // LOGGINGW_H_