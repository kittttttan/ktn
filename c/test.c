#include "string.h"
#include "logger.h"

#ifdef _MSC_VER
#include <time.h>
#include <crtdbg.h>
#endif

int main() {
  TCHAR filename[64];

#ifdef _MSC_VER
  clock_t t, t0 = clock();
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

  loggerSetFilename(_T("test.log"));

  loggerLog(LOG_INFO, _T("info %d\n"), __LINE__);

  loggerSetLevel(LOG_NONE);
  loggerLog(LOG_INFO, _T("none %d\n"), __LINE__);

  loggerSetLevel(LOG_ALL);
  LOGGER_WARN(_T("WARN %d\n"), __LINE__);
  LOGGER_ERROR(_T("ERROR %d\n"), __LINE__);
  LOGGER_INFO(_T("日本語\n"));

  loggerGetFilename(filename, 63);
  TRACE(_T("output: %s ...\n"), filename);

  loggerClose();

#if _MSC_VER
  t = clock();
  TRACE(_T("%dms\n"), t - t0);
  system("pause");
#endif

  return 0;
}