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
#define LOGGING_ERROR(logging, logstr)  LOGGING_LOG(logging, LOG_ERROR, logstr)
#define LOGGING_WARN(logging, logstr)   LOGGING_LOG(logging, LOG_WARN, logstr)
#define LOGGING_INFO(logging, logstr)   LOGGING_LOG(logging, LOG_INFO, logstr)
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
        logLevel_(LOG_INFO),
        logFormat_(&defaultLogFormat),
        filename_("log.txt") {}
    explicit Logging(ILogFormat* format) :
        logLevel_(LOG_INFO),
        logFormat_(format),
        filename_("log.txt") {}
    ~Logging() {}

    LogLevel logLevel() const { return logLevel_; }
    void logLevel(LogLevel logLevel) { logLevel_ = logLevel; }

    void log(const String& log, LogLevel level,
            const char* file, int line, const char* func) {
        String str = logFormat_->format(log, level, file, line, func);
        str.out();

        FILE* fp = fopen(filename_, "a");
        if (!fp) { return; }
        fprintf(fp, "%s\n", str.string());
        fclose(fp);
    }

private:
    LogLevel logLevel_;
    ILogFormat* logFormat_;
    char* filename_;
};

DefaultLogFormat Logging::defaultLogFormat;

} // namespace ktn

#endif // LOGGING_H_