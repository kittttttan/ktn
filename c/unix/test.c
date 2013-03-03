#include "string.h"
#include "logger.h"

int main(int argc, char** argv) {
  char filename[64];

  loggerSetFilename("test.log");

  loggerLog(LOG_INFO, "info %d\n", __LINE__);

  loggerSetLevel(LOG_NONE);
  loggerLog(LOG_INFO, "none %d\n", __LINE__);

  loggerSetLevel(LOG_ALL);
  LOGGER_WARN("WARN %d\n", __LINE__);
  LOGGER_ERROR("ERROR %d\n", __LINE__);
  LOGGER_INFO0("日本語\n");

  loggerGetFilename(filename, 63);
  TRACE("output: %s ...\n", filename);

  loggerClose();

  return 0;
}