#ifndef LOGFORMAT_H_
#define LOGFORMAT_H_

#include <time.h>
#include "string.h"

#ifndef _MSC_VER
#define _stprintf_s(buf, size, ...) _stprintf(buf, __VA_ARGS__)
#endif

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
            const TCHAR* file, int line, const TCHAR* func) = 0;
};

class DefaultLogFormat : public ILogFormat {
public:
    static const TCHAR* logLevelName[];

    DefaultLogFormat() {}
    ~DefaultLogFormat() {}

    String format(const String& log, LogLevel level,
            const TCHAR* file, int line, const TCHAR* func) {
        TCHAR buf[99];
        if (_stprintf_s(buf, 99, _T("%07ld %s[%d]%s %s "),
                clock(), file, line, func, logLevelName[level]) == 99) {
            TRACE(_T("filled buffer"));
        }
        String str(buf);
        return str + log;
    }
};

const TCHAR* DefaultLogFormat::logLevelName[] = {
    _T("NONE"),
    _T("ERROR"),
    _T("WARN"),
    _T("INFO"),
    _T("ALL"),
};
} // namespace ktn

#endif // LOGFORMAT_H_