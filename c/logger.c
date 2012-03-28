#include "logger.h"

#include <stdarg.h>
#include <locale.h>

#define LOCALE_CHAR		32

static char locale_[LOCALE_CHAR] = "Japanese";
static LogLevel loglevel_ = LOG_ALL;
static TCHAR filename_[MAX_PATH] = _T("log.txt");

void loggerSetLocal(const char* locale) {
	strcpy_s(locale_, LOCALE_CHAR - 1, locale);
}

void loggerSetLevel(LogLevel level) {
	loglevel_ = level;
}

void loggerSetFilename(const TCHAR* filename) {
	_tcscpy_s(filename_, MAX_PATH - 1, filename);
}

void loggerLog(int level, const TCHAR* fmt, ...) {
	FILE* f;
	errno_t err;
	va_list ap;
	if (level > loglevel_) { return; }

	va_start(ap, fmt);
	err = _tfopen_s(&f, filename_, _T("a+"));
	if (err != 0) {
		_ftprintf_s(stderr, _T("Failed to open %s\n"), filename_);
		va_end(ap);
		return;
	}

	setlocale(LC_ALL, locale_);
	_vftprintf_s(f, fmt, ap);

	fclose(f);
	va_end(ap);
}