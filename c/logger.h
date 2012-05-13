#ifndef CKTN_LOGGER_H_
#define CKTN_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#ifdef _DEBUG
#define TRACE(fmt, ...) { _tprintf(fmt, __VA_ARGS__); }
#else
#define TRACE(fmt, ...)
#endif

typedef enum _LogLevel {
	LOG_NONE = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_ALL,
} LogLevel;

void loggerGetLocal(char* locale, int size);
LogLevel loggerGetLevel();
void loggerGetFilename(TCHAR* filename, int size);
int loggerGetStdoutFlag();

void loggerSetLocal(const char* locale);
void loggerSetLevel(LogLevel level);
void loggerSetFilename(const TCHAR* filename);
void loggerSetStdoutFlag(int flag);

void loggerLog(LogLevel level, const TCHAR* fmt, ...);

#define LOGGER_INFO(fmt, ...) { loggerLog(LOG_INFO, fmt, __VA_ARGS__); }
#define LOGGER_WARN(fmt, ...) { loggerLog(LOG_WARN, fmt, __VA_ARGS__); }
#define LOGGER_ERROR(fmt, ...) { loggerLog(LOG_ERROR, fmt, __VA_ARGS__); }
#define LOGGER_ALL(fmt, ...) { loggerLog(LOG_ALL, fmt, __VA_ARGS__); }

#ifdef __cplusplus
}
#endif

#endif // CKTN_LOGGER_H_