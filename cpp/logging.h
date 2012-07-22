#ifndef LOGGING_H_
#define LOGGING_H_

#include "logformat.h"

namespace ktn {

#define LOGGING
#ifdef LOGGING
#define LOGGING_LOG(logging, level, logstr)                                     \
    {                                                                           \
        if (logging.logLevel() >= level) {                                      \
            logging.log(logstr, level, _T(__FILE__), __LINE__, _T(__func__));   \
        }                                                                       \
    }
#define LOGGING_ERROR(logging, logstr)  LOGGING_LOG(logging, LOG_ERROR, logstr)
#define LOGGING_WARN(logging, logstr)   LOGGING_LOG(logging, LOG_WARN, logstr)
#define LOGGING_INFO(logging, logstr)   LOGGING_LOG(logging, LOG_INFO, logstr)
#else
#define LOGGING_LOG(logging, level, logstr)
#define LOGGING_ERROR(logging, logstr)
#define LOGGING_WARN(logging, logstr)
#define LOGGING_INFO(logging, logstr)
#endif

class Logging {
public:
    static DefaultLogFormat defaultLogFormat;

    Logging() :
        logLevel_(LOG_INFO), logFormat_(&defaultLogFormat) {}
    explicit Logging(ILogFormat* format) :
        logLevel_(LOG_INFO), logFormat_(format) {}
    ~Logging() {}

    LogLevel logLevel() const { return logLevel_; }
    void logLevel(LogLevel logLevel) { logLevel_ = logLevel; }

    void log(const String& log, LogLevel level,
            const TCHAR* file, int line, const TCHAR* func) {
        logFormat_->format(log, level, file, line, func).out();
    }

private:
    LogLevel logLevel_;
    ILogFormat* logFormat_;
};

DefaultLogFormat Logging::defaultLogFormat;

} // namespace ktn

#endif // LOGGING_H_