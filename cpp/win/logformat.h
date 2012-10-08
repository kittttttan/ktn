#pragma once
#ifndef LOGFORMAT_H_
#define LOGFORMAT_H_

#include "string.h"
#include "loglevel.h"
#include <ctime>
#include <sstream>
#include <iomanip>

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
        std::ostringstream os;
        os << std::setw(7) << std::setfill('0') << clock() << " "
           << file << "[" << line << "]"
           << func << " " << LogLevel::LevelName[level] << " "
           << log;
        return String(os.str());
    }
};

} // namespace ktn

#endif // LOGFORMAT_H_