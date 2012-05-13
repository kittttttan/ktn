#include "string.h"
#include "logger.h"

int main() {
	TCHAR filename[64];
	loggerLog(LOG_INFO, _T("てすと %d\n"), __LINE__);

	loggerSetLevel(LOG_NONE);
	loggerLog(LOG_INFO, _T("テスト %d\n"), __LINE__);

	loggerSetLevel(LOG_ALL);
	LOGGER_WARN(_T("WARN %d\n"), __LINE__);
	LOGGER_ERROR(_T("ERROR %d\n"), __LINE__);

	loggerGetFilename(filename, 63);
	TRACE(_T("出力: %s ...\n"), filename);
	getchar();

	return 0;
}