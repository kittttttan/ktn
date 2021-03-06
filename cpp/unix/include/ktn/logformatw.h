#pragma once
#ifndef LOGFORMATW_H_
#define LOGFORMATW_H_

#include "ktn/loglevel.h"
#include "ktn/stringw.h"
#include <stdio.h>
#include <time.h>

#ifdef __MINGW32__
#define swprintf(dest, size, format, ...) swprintf(dest, format, __VA_ARGS__)
#endif

namespace ktn {

class ILogFormatW {
public:
    virtual ~ILogFormatW() {}
    virtual StringW format(const StringW& log, LogLevel::Level level,
            const char* file, int line, const wchar_t* func) = 0;
};

class DefaultLogFormatW : public ILogFormatW {
public:
    static const wchar_t* logLevelName[];

    DefaultLogFormatW() {}
    ~DefaultLogFormatW() {}

    StringW format(const StringW& log, LogLevel::Level level,
            const char* file, int line, const wchar_t* func) {
        wchar_t buf[99];
        if (swprintf(buf, 99, L"%07ld %s[%d]%ls %ls ",
                clock(), file, line, func, LogLevel::LevelNameW[level]) < 0) {
            TRACE("filled buffer");
        }
        StringW str(buf);
        return str + log;
    }
};

} // namespace ktn

#endif // LOGFORMATW_H_