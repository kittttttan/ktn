#include "ktn/date_format.h"
#include "ktn/date.h"

namespace ktn {

std::string DateFormat::format(const Date& date) const
{
    char datetime[32];
    const struct tm* d = date.tm();
    strftime(datetime, 31, "%Y-%m-%d %H:%M:%S", d);

    long sec;
    errno_t err = _get_timezone(&sec);
    if (err) {
        throw std::runtime_error("Failed _get_timezone");
    }

    char timezone[64];
    sprintf_s(timezone, "%s %+02.2ld00", datetime, -sec / 3600);

    return std::string(timezone);
}

std::wstring DateFormat::wformat(const Date& date) const
{
    wchar_t datetime[21];
    const struct tm* d = date.tm();
    wcsftime(datetime, 20, L"%Y-%m-%d %H:%M:%S", d);

    long sec;
    errno_t err = _get_timezone(&sec);
    if (err) {
        throw std::runtime_error("Failed _get_timezone");
    }

    wchar_t timezone[32];
    swprintf(timezone, 31, L"%s %+02.2ld00", datetime, -sec / 3600);

    return std::wstring(timezone);
}

} // namespace ktn