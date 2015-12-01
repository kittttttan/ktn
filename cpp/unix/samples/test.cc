/**
 * @file  cpp/test.cc
 * @brief test for String
 */
#include <ktn/dbg.h>
#include <ktn/loggingw.h>
#include <ktn/stringw.h>
#include <ktn/stringw.h>
#include <ktn/date.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

using namespace ktn;

void stringTest() {
    setlocale(LC_CTYPE, "");

    StringW a(L"水樹"), b(L"奈々");
    wprintf(L"%ls: %d\n", a.string(), a.length());
    wprintf(L"%c\n", a[-1]);

    StringW c = a + b;
    c.out();
    (-c).out();
    std::wcout << c << std::endl;

    wprintf(L"%d\n", c.indexOf(L'々'));
    wprintf(L"%d\n", c.lastIndexOf(L'水', 2));
    wprintf(L"%ls\n", c.slice(2).string());
    wprintf(L"%ls\n", c.slice(0, -3).string());
    wprintf(L"%ls\n", c.substr(2, 2).string());

    StringW space(L"  Nana  Mizuki   ");
    wprintf(L"'%ls'\n", space.toUpperCase().string());
    wprintf(L"'%ls'\n", space.toLowerCase().string());
    wprintf(L"'%ls'\n", space.trimLeft().string());
    wprintf(L"'%ls'\n", space.trimRight().string());
    wprintf(L"'%ls'\n", space.trim().string());
    wprintf(L"'%ls'\n", (space - L' ').string());

    StringW d = b * 7;
    d.out();

    d = b * -3;
    d.out();
}

void dateTest() {
  /*
    Date d;
    std::wcout << d << std::endl;
    std::wcout << (d + Date::MINUTE) << std::endl;
    std::wcout << (d - Date::DAY) << std::endl;

    d += Date::WEEK;
    std::wcout << d << std::endl;

    Date d2(d);
    d2.setMonth(2);
    std::wcout << d2 << std::endl;

    d2.addMonth(-2);
    std::wcout << d2 << std::endl;

    d2.setDateFormat("%Y/%m/%d");
    std::wcout << d2 << std::endl;

    std::wcout.setf(std::ios::boolalpha);
    std::wcout << L"d < d2 == " << (d < d2) << std::endl;

    std::wcout << Date::parse("1980-01-21") << std::endl;
    std::wcout << Date::parse("2000") << std::endl;
  */
}

int main(int argc, const char** argv) {
    stringTest();
    dateTest();

    return 0;
}
