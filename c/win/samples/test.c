#include <ktn/string.h>
#include <ktn/logger.h>

#include <time.h>
#include <crtdbg.h>

int main() {
  TCHAR filename[64];

  clock_t t, t0 = clock();
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

  loggerSetFilename(_T("test.log"));

  loggerLog(LOG_INFO, _T("info %d\n"), __LINE__);

  loggerSetLevel(LOG_NONE);
  loggerLog(LOG_INFO, _T("none %d\n"), __LINE__);

  loggerSetLevel(LOG_ALL);
  LOGGER_WARN(_T("WARN %d\n"), __LINE__);
  LOGGER_ERROR(_T("ERROR %d\n"), __LINE__);
  LOGGER_INFO0(_T("日本語\n"));

  loggerGetFilename(filename, 63);
  TRACE(_T("output: %s ...\n"), filename);

  loggerClose();

  t = clock();
  TRACE(_T("%dms\n"), t - t0);
  system("pause");

  return 0;
}