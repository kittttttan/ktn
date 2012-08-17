#pragma once
#ifndef LOGGINGW_H_
#define LOGGINGW_H_

#include "logformatw.h"
#include "dbg.h"

#define LOGGINGW
#ifdef LOGGINGW
#define LOGGINGW_LOG(logging, level, logstr)                                 \
    {                                                                        \
        if (logging.logLevel() >= level) {                                   \
            logging.log(logstr, level, _T(__FILE__), __LINE__, _T(__func__));\
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
        filename_(L"log.txt") {}
    explicit LoggingW(ILogFormatW* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_(L"log.txt") {}
    ~LoggingW() {}

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const StringW& log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func) {
        StringW str = logFormat_->format(log, level, file, line, func);
        str.out();

        FILE* fp = nullptr;
        errno_t err = _wfopen_s(&fp, filename_, L"a,ccs=UTF-8");
        if (err) { return; }
        fwprintf(fp, L"%s\n", str.string());
        fclose(fp);
    }

private:
    LogLevel::Level logLevel_;
    ILogFormatW* logFormat_;
    wchar_t* filename_;
};

DefaultLogFormatW LoggingW::defaultLogFormat;

} // namespace ktn

#endif // LOGGINGW_H_