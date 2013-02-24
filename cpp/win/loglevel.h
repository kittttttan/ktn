#pragma once
#ifndef LOGLEVEL_H_
#define LOGLEVEL_H_

namespace ktn {

namespace LogLevel {
    static enum Level {
        NONE = 0,   //< no log
        ERR,        //< define ERROR in wingdi.h
        WARN,       //< Warning
        INFO,       //< Infomation
        ALL,        //< All
    };

    static const char* LevelName[] = {
        "NONE",
        "ERROR",
        "WARN",
        "INFO",
        "ALL",
    };

    static const wchar_t* LevelNameW[] = {
        L"NONE",
        L"ERROR",
        L"WARN",
        L"INFO",
        L"ALL",
    };

} // namespace LogLevel

} // namespace ktn

#endif // LOGLEVEL_H_