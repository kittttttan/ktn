#include "logger.h"

#include <stdarg.h>
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
#define LOG_COLOR_RED     (FOREGROUND_RED)
#define LOG_COLOR_GREEN   (FOREGROUND_GREEN)
#define LOG_COLOR_YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN)
#define LOG_COLOR_CYAN    (FOREGROUND_BLUE | FOREGROUND_GREEN)
#else
#define LOG_COLOR_RED     (1)
#define LOG_COLOR_GREEN   (2)
#define LOG_COLOR_YELLOW  (3)
#define LOG_COLOR_CYAN    (6)

#define strcpy_s(dest, size, src)   strcpy(dest, src)
#define _tcscpy_s(dest, size, src)  _tcscpy(dest, src)
#define _ftprintf_s                 _ftprintf
#define _vftprintf_s                _vftprintf
#endif

static char locale_[LOCALE_CHAR] = "Japanese";
static LogLevel loglevel_ = LOG_ALL;
static TCHAR filename_[MAX_PATH] = _T("log.txt");
static int stdout_ = 1;

static int getColor(LogLevel level);
static void _vtprintfColored(int color, const TCHAR* fmt, va_list ap);

void loggerGetLocal(char* locale, int size) {
  const int s = min(size, LOCALE_CHAR);
  strcpy_s(locale, s, locale_);
}

LogLevel loggerGetLevel() {
  return loglevel_;
}

void loggerGetFilename(TCHAR* filename, int size) {
  const int s = min(size, MAX_PATH);
  _tcscpy_s(filename, s, filename_);
}

int loggerGetStdoutFlag() {
  return stdout_;
}

void loggerSetLocal(const char* locale) {
  strcpy_s(locale_, LOCALE_CHAR - 1, locale);
}

void loggerSetLevel(LogLevel level) {
  loglevel_ = level;
}

void loggerSetFilename(const TCHAR* filename) {
  _tcscpy_s(filename_, MAX_PATH - 1, filename);
}

void loggerSetStdoutFlag(int flag) {
  stdout_ = flag;
}

void loggerLog(LogLevel level, const TCHAR* fmt, ...) {
  FILE* f;
#ifdef _MSC_VER
  errno_t err;
#endif
  va_list ap;
  if (level > loglevel_) { return; }

  va_start(ap, fmt);

  setlocale(LC_ALL, locale_);
  if (stdout_) {
    _vtprintfColored(getColor(level), fmt, ap);
  }

#ifdef _MSC_VER
  err = _tfopen_s(&f, filename_, _T("a+"));
  if (err != 0) {
#else
  f = _tfopen(filename_, _T("a+"));
  if (!f) {
#endif
    _ftprintf_s(stderr, _T("Failed to open %s\n"), filename_);
    va_end(ap);
    return;
  }
  _vftprintf_s(f, fmt, ap);

  fclose(f);
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
    color | FOREGROUND_INTENSITY);

  _vtprintf(fmt, ap);

  fflush(stdout);
  SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
  printf("\033[0;3%dm", color);
  _vtprintf(fmt, ap);
  printf("\033[m");
#endif
}