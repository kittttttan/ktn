#pragma once
#ifndef LOGGING_H_
#define LOGGING_H_

#include "logformat.h"
#include "dbg.h"

#define LOGGING
#ifdef LOGGING
#define LOGGING_LOG(logging, level, logstr)                             \
    {                                                                   \
        if (logging.logLevel() >= level) {                              \
            logging.log(logstr, level, __FILE__, __LINE__, __func__);   \
        }                                                               \
    }
#define LOGGING_ERROR(logging, logstr)  LOGGING_LOG(logging, LogLevel::ERROR, logstr)
#define LOGGING_WARN(logging, logstr)   LOGGING_LOG(logging, LogLevel::WARN, logstr)
#define LOGGING_INFO(logging, logstr)   LOGGING_LOG(logging, LogLevel::INFO, logstr)
#else
#define LOGGING_LOG(logging, level, logstr)
#define LOGGING_ERROR(logging, logstr)
#define LOGGING_WARN(logging, logstr)
#define LOGGING_INFO(logging, logstr)
#endif

namespace ktn {

class Logging {
public:
    static DefaultLogFormat defaultLogFormat;

    Logging() :
        logLevel_(LogLevel::INFO),
        logFormat_(&defaultLogFormat),
        filename_("log.txt") {}
    explicit Logging(ILogFormat* format) :
        logLevel_(LogLevel::INFO),
        logFormat_(format),
        filename_("log.txt") {}
    ~Logging() {}

    LogLevel::Level logLevel() const { return logLevel_; }
    void logLevel(LogLevel::Level logLevel) { logLevel_ = logLevel; }

    void log(const String& log, LogLevel::Level level,
            const char* file, int line, const char* func) {
        String str = logFormat_->format(log, level, file, line, func);
        str.out();

        FILE* fp = nullptr;
        errno_t err = fopen_s(&fp, filename_, "a");
        if (err) { return; }
        fprintf(fp, "%s\n", str.string());
        fclose(fp);
    }

private:
    LogLevel::Level logLevel_;
    ILogFormat* logFormat_;
    char* filename_;
};

DefaultLogFormat Logging::defaultLogFormat;

} // namespace ktn

#endif // LOGGING_H_