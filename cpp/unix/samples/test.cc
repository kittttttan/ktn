/**
 * @file  cpp/test.cc
 * @brief test for String
 */
#include <ktn/dbg.h>
#include <ktn/loggerw.h>
#include <ktn/stringw.h>
#include <ktn/date.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <iostream>

using namespace ktn;
using namespace std;

void stringTest() {
    wcout << L"## stringTest" << endl;

    StringW a(L"水樹"), b(L"奈々");
    wprintf(L"%ls: %d\n", a.string(), a.length());
    wprintf(L"%c\n", a[-1]);

    StringW c = a + b;
    c.out();
    (-c).out();
    wcout << c << endl;

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
    cout << "## dateTest" << endl;

    Date d;
    cout << d << endl;
    cout << (d + Date::MINUTE) << endl;
    cout << (d - Date::DAY) << endl;

    d += Date::WEEK;
    cout << d << endl;

    Date d2(d);
    //d2.setMonth(2);
    //cout << d2 << endl;

    d2.addMonth(-2);
    cout << d2 << endl;

    //d2.setDateFormat("%Y/%m/%d");
    //cout << d2 << endl;

    cout.setf(ios::boolalpha);
    cout << "d < d2 == " << (d < d2) << endl;

    cout << Date::parse("1980-01-21") << endl;
    cout << Date::parse("2000") << endl;
}

void defaultLocale() {
    ios_base::sync_with_stdio(false);
    locale default_loc("");
    locale::global(default_loc);
    locale ctype_default(locale::classic(), default_loc, locale::ctype);
    wcout.imbue(ctype_default);
    wcin.imbue(ctype_default);
}

int main(int argc, const char** argv) {
    defaultLocale();

    stringTest();
    dateTest();

    return 0;
}
