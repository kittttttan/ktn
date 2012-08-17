#pragma once
#ifndef LOGFORMATW_H_
#define LOGFORMATW_H_

#include <ctime>
#include "stringw.h"
#include "loglevel.h"

namespace ktn {

class ILogFormatW {
public:
    virtual ~ILogFormatW() {}
    virtual StringW format(const StringW& log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func) = 0;
};

class DefaultLogFormatW : public ILogFormatW {
public:
    static const wchar_t* logLevelName[];

    DefaultLogFormatW() {}
    ~DefaultLogFormatW() {}

    StringW format(const StringW& log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func) {
        wchar_t buf[99];
        if (swprintf_s(buf, 99, L"%07ld %s[%d]%s %s ",
                clock(), file, line, func, LogLevel::LevelNameW[level]) == 99) {
            TRACE("filled buffer");
        }
        StringW str(buf);
        return str + log;
    }
};

} // namespace ktn

#endif // LOGFORMATW_H_