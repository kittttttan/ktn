#include "string.h"
#include "logger.h"

int main() {
	loggerLog(LOG_INFO, _T("てすと\n"));

	loggerSetLevel(LOG_NONE);
	loggerLog(LOG_INFO, _T("テスト %d\n"), 2);

	loggerSetLevel(LOG_INFO);
	LOGGER_INFO(_T("テスト %d\n"), 3);

	return 0;
}