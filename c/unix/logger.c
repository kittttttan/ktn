#include "logger.h"

#include <locale.h>

#ifndef min
#define min(a,b)  (((a)<(b))?(a):(b))
#endif

enum {
  LOCALE_CHAR = 32,
#ifndef MAX_PATH
  MAX_PATH = 260,
#endif
};

enum {
  LOG_COLOR_NONE,
  LOG_COLOR_RED,
  LOG_COLOR_GREEN,
  LOG_COLOR_YELLOW,
  LOG_COLOR_CYAN = 6,
};

static char locale_[LOCALE_CHAR] = "";
static LogLevel loglevel_ = LOG_ALL;
static char filename_[MAX_PATH] = "";
static int format_ = LOG_OUT_STDOUT;
static FILE* fp_ = NULL;

static int getColor(LogLevel level);
static void printfColored(int color, const char* fmt);
static void vprintfColored(int color, const char* fmt, va_list ap);

void loggerGetLocal(char* locale, int size) {
    strcpy(locale, locale_);
}

LogLevel loggerGetLevel() {
    return loglevel_;
}

void loggerGetFilename(char* filename, int size) {
    strcpy(filename, filename_);
}

int loggerGetLogFormat() {
    return format_;
}

void loggerSetLogFormat(int format) {
    format_ = format;
}

void loggerSetLocal(const char* locale) {
    strcpy(locale_, locale);
}

void loggerSetLevel(LogLevel level) {
    loglevel_ = level;
}

void loggerSetFilename(const char* filename) {
    setlocale(LC_ALL, locale_);

    if (!filename) {
        format_ &= ~LOG_OUT_FILE;
        return;
    } else {
        strcpy(filename_, filename);
    }

    fp_ = fopen(filename_, "a+");

    if (!fp_) {
        fprintf(stderr, "Failed to open %s\n", filename_);
        format_ &= ~LOG_OUT_FILE;
    } else {
        format_ |= LOG_OUT_FILE;
    }
}

void loggerClose() {
    if (fp_) { fclose(fp_); }
}

void loggerLog0(LogLevel level, const char* fmt) {
	if (level > loglevel_) { return; }

    if (format_ & LOG_OUT_STDOUT) {
        printfColored(getColor(level), fmt);
    }

    if (format_ & LOG_OUT_FILE) {
        fprintf(fp_, fmt);
    }
}

void loggerLog(LogLevel level, const char* fmt, ...) {
    va_list ap;
    if (level > loglevel_) { return; }

    va_start(ap, fmt);

    if (format_ & LOG_OUT_STDOUT) {
        vprintfColored(getColor(level), fmt, ap);
    }

    if (format_ & LOG_OUT_FILE) {
        vfprintf(fp_, fmt, ap);
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

void printfColored(int color, const char* fmt) {
    if (color == LOG_COLOR_NONE) {
        printf(fmt);
        return;
    }

    printf("\033[0;3%dm", color);
    printf(fmt);
    printf("\033[m");
}

void vprintfColored(int color, const char* fmt, va_list ap) {
    if (color == LOG_COLOR_NONE) {
        vprintf(fmt, ap);
        return;
    }

    printf("\033[0;3%dm", color);
    vprintf(fmt, ap);
    printf("\033[m");
}
