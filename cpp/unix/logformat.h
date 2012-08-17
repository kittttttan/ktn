#ifndef LOGFORMAT_H_
#define LOGFORMAT_H_

#include <time.h>
#include "string.h"

namespace ktn {

enum LogLevel {
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_ALL,
};

class ILogFormat {
public:
    virtual ~ILogFormat() {}
    virtual String format(const String& log, LogLevel level,
            const char* file, int line, const char* func) = 0;
};

class DefaultLogFormat : public ILogFormat {
public:
    static const char* logLevelName[];

    DefaultLogFormat() {}
    ~DefaultLogFormat() {}

    String format(const String& log, LogLevel level,
            const char* file, int line, const char* func) {
        char buf[99];
        if (sprintf(buf, "%07ld %s[%d]%s %s ",
                clock(), file, line, func, logLevelName[level]) == EOF) {
            TRACE("filled buffer");
        }
        String str(buf);
        return str + log;
    }
};

const char* DefaultLogFormat::logLevelName[] = {
    "NONE",
    "ERROR",
    "WARN",
    "INFO",
    "ALL",
};
} // namespace ktn

#endif // LOGFORMAT_H_