#pragma once

#include "stringw.h"
#include "loglevel.h"
#include <ctime>

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
        std::wostringstream os;
        os << std::setw(7) << std::setfill(L'0') << clock() << L" "
           << file << L"[" << line << L"]"
           << func << L" " << LogLevel::LevelNameW[level] << L" "
           << log;
        return StringW(os.str());
    }
};

} // namespace ktn
