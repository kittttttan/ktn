#pragma once

//#include "ktn/logger.h"

namespace ktn {

DefaultLogFormat Logger::defaultLogFormat;
const DWORD Logger::maxWaitTime = 3000;

std::string Logger::name_ = "";
HANDLE Logger::hWriteEvent_ = nullptr;
HANDLE Logger::hThread_ = nullptr;

inline Logger* Logger::getInstance(const char* name)
{
    if (name) { name_ = name; }
    static Logger logger;
    return &logger;
}

inline Logger::Logger() :
    logLevel_(LogLevel::INFO),
    logFormat_(&defaultLogFormat),
    filename_("ktn.log")
{
    hWriteEvent_ = ::CreateEventA(NULL, FALSE, TRUE, "LogWriteEvent");
}

inline Logger::~Logger()
{
    DWORD dwWait = ::WaitForSingleObject(hWriteEvent_, maxWaitTime);
    if (dwWait != WAIT_OBJECT_0) {
        fprintf(stderr, "failed WaitForSingleObject\n");
    }

    if (hWriteEvent_) {
        ::CloseHandle(hWriteEvent_);
        hWriteEvent_ = nullptr;
    }

    if (hThread_) {
        ::CloseHandle(hThread_);
        hThread_ = nullptr;
    }
}

inline void Logger::process()
{
    ::ResetEvent(hWriteEvent_);

    getInstance(nullptr)->write();

    ::SetEvent(hWriteEvent_);
    ::ExitThread(0);
}

inline void Logger::log(const char* log, LogLevel::Level level,
        const char* file, int line, const char* func)
{
    std::string str = logFormat_->format(log, level, file, name_.c_str(), line, func);

    DWORD dwWait = ::WaitForSingleObject(hWriteEvent_, maxWaitTime);
    if (dwWait != WAIT_OBJECT_0) {
        fprintf(stderr, "failed WaitForSingleObject\n");
    }

    queue_.push(str);
    hThread_ = ::CreateThread(NULL, 0,
                (LPTHREAD_START_ROUTINE)process,
                0, 0, nullptr);
}

inline void Logger::write()
{
    if (queue_.empty()) { return; }

    std::string& str = queue_.front();
    std::cout << str << std::endl;

    std::ofstream ofs(filename_, std::ios::app);
    ofs << str << std::endl;

    queue_.pop();
}

} // namespace ktn