#pragma once
#ifndef LOGFORMAT_H_
#define LOGFORMAT_H_

#include <ctime>
#include "string.h"
#include "loglevel.h"

namespace ktn {

class ILogFormat {
public:
    virtual ~ILogFormat() {}
    virtual String format(const String& log, LogLevel::Level level,
            const char* file, int line, const char* func) = 0;
};

class DefaultLogFormat : public ILogFormat {
public:
    DefaultLogFormat() {}
    ~DefaultLogFormat() {}

    String format(const String& log, LogLevel::Level level,
            const char* file, int line, const char* func) {
        char buf[99];
        if (sprintf_s(buf, 99, "%07ld %s[%d]%s %s ",
                clock(), file, line, func, LogLevel::LevelName[level]) == 99) {
            TRACE("filled buffer");
        }
        String str(buf);
        return str + log;
    }
};

} // namespace ktn

#endif // LOGFORMAT_H_