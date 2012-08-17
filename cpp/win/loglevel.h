#pragma once
#ifndef LOGLEVEL_H_
#define LOGLEVEL_H_

namespace ktn {

namespace LogLevel {
    enum Level {
        NONE = 0,
        ERROR,
        WARN,
        INFO,
        ALL,
    };

    const char* LevelName[] = {
        "NONE",
        "ERROR",
        "WARN",
        "INFO",
        "ALL",
    };

    const wchar_t* LevelNameW[] = {
        L"NONE",
        L"ERROR",
        L"WARN",
        L"INFO",
        L"ALL",
    };

} // namespace LogLevel

} // namespace ktn

#endif // LOGLEVEL_H_