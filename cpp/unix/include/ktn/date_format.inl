#pragma once

//#include "ktn/date_format.h"
#include <stdio.h>
#include <time.h>

namespace ktn {

inline std::string DateFormat::format(const BaseDate& date) const
{
    char datetime[32];
    const struct tm* d = date.tm();
    strftime(datetime, 31, "%Y-%m-%d %H:%M:%S", d);

    //long sec;
    //errno_t err = _get_timezone(&sec);

    char timezone[64];
    sprintf(timezone, "%s", datetime);
    //sprintf_s(timezone, "%s %+02.2ld00", datetime, -sec / 3600);

    return std::string(timezone);
}

inline std::wstring DateFormat::wformat(const BaseDate& date) const
{
    wchar_t datetime[21];
    const struct tm* d = date.tm();
    wcsftime(datetime, 20, L"%Y-%m-%d %H:%M:%S", d);

    //long sec;
    //errno_t err = _get_timezone(&sec);

    wchar_t timezone[32];
    //swprintf(timezone, L"%s", datetime);
    swprintf(timezone, 31, L"%s", datetime);
    //swprintf(timezone, 31, L"%s %+02.2ld00", datetime, -sec / 3600);

    return std::wstring(timezone);
}

} // namespace ktn