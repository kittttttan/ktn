#include "string.h"
#include "logger.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main() {
  TCHAR filename[64];

#ifdef _MSC_VER
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

  loggerLog(LOG_INFO, _T("info %d\n"), __LINE__);

  loggerSetLevel(LOG_NONE);
  loggerLog(LOG_INFO, _T("none %d\n"), __LINE__);

  loggerSetLevel(LOG_ALL);
  LOGGER_WARN(_T("WARN %d\n"), __LINE__);
  LOGGER_ERROR(_T("ERROR %d\n"), __LINE__);

  loggerGetFilename(filename, 63);
  TRACE(_T("output: %s ...\n"), filename);
#if _MSC_VER
  getchar();
#endif

  return 0;
}