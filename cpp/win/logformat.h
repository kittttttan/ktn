#pragma once
#ifndef LOGFORMAT_H_
#define LOGFORMAT_H_

#include "loglevel.h"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace ktn {

class ILogFormat {
public:
    virtual ~ILogFormat() {}
    virtual std::string format(const char* log, LogLevel::Level level,
            const char* file, const char* name, int line, const char* func) = 0;
};

class DefaultLogFormat : public ILogFormat {
public:
    DefaultLogFormat() {}
    ~DefaultLogFormat() {}

    std::string format(const char* log, LogLevel::Level level,
            const char* file, const char* name, int line, const char* func) {
        std::ostringstream os;
        os << std::setw(7) << std::setfill('0') << clock() << " ";
        if (line >= 0) {
           os << file << "[" << line << "]";
        }
        os << func << " "
           << name << " "
           << LogLevel::LevelName[level] << " "
           << log;
        return std::string(os.str());
    }
};

} // namespace ktn

#endif // LOGFORMAT_H_