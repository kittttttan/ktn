#include "logger.h"

#include <locale.h>

#define LOCALE_CHAR     (32)

#ifndef MAX_PATH
#define MAX_PATH        (260)
#endif

#ifndef min
#define min(a,b)        (((a)<(b))?(a):(b))
#endif

#define LOG_COLOR_NONE  (0)
#ifdef _MSC_VER
#include <share.h>
#define LOG_COLOR_RED     (FOREGROUND_RED)
#define LOG_COLOR_GREEN   (FOREGROUND_GREEN)
#define LOG_COLOR_YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN)
#define LOG_COLOR_CYAN    (FOREGROUND_BLUE | FOREGROUND_GREEN)
#else
#define LOG_COLOR_RED     (1)
#define LOG_COLOR_GREEN   (2)
#define LOG_COLOR_YELLOW  (3)
#define LOG_COLOR_CYAN    (6)
#define _ftprintf_s                 _ftprintf
#define _vftprintf_s                _vftprintf
#endif

static char locale_[LOCALE_CHAR] = "";
static LogLevel loglevel_ = LOG_ALL;
static TCHAR filename_[MAX_PATH] = _T("");
static int format_ = LOG_OUT_STDOUT;
static FILE* fp_ = NULL;

static int getColor(LogLevel level);
static void _tprintfColored(int color, const TCHAR* fmt);
static void _vtprintfColored(int color, const TCHAR* fmt, va_list ap);

void loggerGetLocal(char* locale, int size) {
#ifdef _MSC_VER
    const int s = min(size, LOCALE_CHAR);
    strcpy_s(locale, s, locale_);
#else
    strcpy(locale, locale_);
#endif
}

LogLevel loggerGetLevel() {
    return loglevel_;
}

void loggerGetFilename(TCHAR* filename, int size) {
#ifdef _MSC_VER
    const int s = min(size, MAX_PATH);
    _tcscpy_s(filename, s, filename_);
#else
    _tcscpy(filename, filename_);
#endif
}

int loggerGetLogFormat() {
    return format_;
}

void loggerSetLogFormat(int format) {
    format_ = format;
}

void loggerSetLocal(const char* locale) {
#ifdef _MSC_VER
    strcpy_s(locale_, LOCALE_CHAR - 1, locale);
#else
    strcpy(locale_, locale);
#endif
}

void loggerSetLevel(LogLevel level) {
    loglevel_ = level;
}

void loggerSetFilename(const TCHAR* filename) {
    setlocale(LC_ALL, locale_);

    if (!filename) {
        format_ &= ~LOG_OUT_FILE;
        return;
    } else {
#ifdef _MSC_VER
        _tcscpy_s(filename_, MAX_PATH - 1, filename);
#else
        _tcscpy(filename_, filename);
#endif
    }

#ifdef _MSC_VER
    fp_ = _tfsopen(filename_, _T("a+,ccs=UTF-8"), _SH_DENYNO);
#else
    fp_ = _tfopen(filename_, _T("a+"));
#endif

    if (!fp_) {
        _ftprintf_s(stderr, _T("Failed to open %s\n"), filename_);
        format_ &= ~LOG_OUT_FILE;
    } else {
        format_ |= LOG_OUT_FILE;
    }
}

void loggerClose() {
    if (fp_) { fclose(fp_); }
}

void loggerLog0(LogLevel level, const TCHAR* fmt) {
	if (level > loglevel_) { return; }

    if (format_ & LOG_OUT_STDOUT) {
        _tprintfColored(getColor(level), fmt);
    }

    if (format_ & LOG_OUT_FILE) {
        _ftprintf_s(fp_, fmt);
    }
}

void loggerLog(LogLevel level, const TCHAR* fmt, ...) {
    va_list ap;
    if (level > loglevel_) { return; }

    va_start(ap, fmt);

    if (format_ & LOG_OUT_STDOUT) {
        _vtprintfColored(getColor(level), fmt, ap);
    }

    if (format_ & LOG_OUT_FILE) {
        _vftprintf_s(fp_, fmt, ap);
    }

    va_end(ap);
}

int getColor(LogLevel level) {
    int color;

    switch (level) {
    case LOG_INFO:
        color = LOG_COLOR_CYAN;
        break;
    case LOG_WARN:
        color = LOG_COLOR_YELLOW;
        break;
    case LOG_ERROR:
        color = LOG_COLOR_RED;
        break;
    default:
        color = LOG_COLOR_NONE;
        break;
    }

    return color;
}

void _tprintfColored(int color, const TCHAR* fmt) {
#if _MSC_VER
    WORD old_color_attrs;
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    HANDLE stdout_handle;
#endif

    if (color == LOG_COLOR_NONE) {
        _tprintf(fmt);
        return;
    }

#if _MSC_VER
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
    old_color_attrs = buffer_info.wAttributes;

    fflush(stdout);
    SetConsoleTextAttribute(stdout_handle,
        (WORD)color | FOREGROUND_INTENSITY);

    _tprintf(fmt);

    fflush(stdout);
    SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
    printf("\033[0;3%dm", color);
    _tprintf(fmt);
    printf("\033[m");
#endif
}

void _vtprintfColored(int color, const TCHAR* fmt, va_list ap) {
#if _MSC_VER
    WORD old_color_attrs;
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    HANDLE stdout_handle;
#endif

    if (color == LOG_COLOR_NONE) {
        _vtprintf(fmt, ap);
        return;
    }

#if _MSC_VER
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
    old_color_attrs = buffer_info.wAttributes;

    fflush(stdout);
    SetConsoleTextAttribute(stdout_handle,
        (WORD)color | FOREGROUND_INTENSITY);

    _vtprintf(fmt, ap);

    fflush(stdout);
    SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
    printf("\033[0;3%dm", color);
    _vtprintf(fmt, ap);
    printf("\033[m");
#endif
}
