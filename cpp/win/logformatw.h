#pragma once

#include "loglevel.h"
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

namespace ktn {

class ILogFormatW {
public:
    virtual ~ILogFormatW() {}
    virtual std::wstring format(const wchar_t* log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func) = 0;
};

class DefaultLogFormatW : public ILogFormatW {
public:
    static const wchar_t* logLevelName[];

    DefaultLogFormatW() {}
    ~DefaultLogFormatW() {}

    std::wstring format(const wchar_t* log, LogLevel::Level level,
            const wchar_t* file, int line, const wchar_t* func) {
        std::wostringstream os;
        os << std::setw(7) << std::setfill(L'0') << clock() << L" "
           << file << L"[" << line << L"]"
           << func << L" " << LogLevel::LevelNameW[level] << L" "
           << log;
        return os.str();
    }
};

} // namespace ktn
