#include "loggerw.h"
#include <iostream>
#include <fstream>

namespace ktn {

DefaultLogFormatW LoggerW::defaultLogFormat;

void LoggerW::log(const wchar_t* log, LogLevel::Level level,
        const wchar_t* file, int line, const wchar_t* func)
{
    std::wstring str = logFormat_->format(log, level, file, line, func);
    std::wcout << str << std::endl;

    //std::wofstream ofs(filename_, std::ios::app | std::ios::binary);
    //ofs.imbue(std::locale(ofs.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
    //ofs << str << std::endl;

    FILE* fp = _wfsopen(filename_, L"a+,ccs=UTF-8", _SH_DENYNO);
    if (!fp) {
        fprintf(stderr, "failed open\n");
        return;
    }
    fwprintf_s(fp, L"%ls\n", str.c_str());
    fclose(fp);
}

}
