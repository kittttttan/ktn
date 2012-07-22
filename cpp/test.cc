/**
 * @file  cpp/test.cc
 * @brief test for String
 */
#include "string.h"
#include "array.h"
#include "dbg.h"
#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif

using namespace ktn;

void stringTest() {
#ifdef _UNICODE
    setlocale(LC_CTYPE, "");
#endif

    Logging logging;
    String title(_T("Samples for String class"));
    LOGGING_INFO(logging, title);

    String a(_T("水樹")), b(_T("奈々"));
    _tprintf(_T("%s: %d\n"), a.string(), a.length());
    _tprintf(_T("%c\n"), a[-1]);
    _tprintf(_T("%c\n"), a[7]);

    String c = a + b;
    c.out();
    (-c).out();
    std::_tcout << c << std::endl;

    _tprintf(_T("%d\n"), c.indexOf(_T('々')));
    _tprintf(_T("%d\n"), c.lastIndexOf(_T('水'), 2));
    _tprintf(_T("%s\n"), c.slice(2).string());
    _tprintf(_T("%s\n"), c.slice(0, -3).string());
    _tprintf(_T("%s\n"), c.substr(2, 2).string());

    String space(_T("  Nana  Mizuki   "));
    _tprintf(_T("'%s'\n"), space.toUpperCase().string());
    _tprintf(_T("'%s'\n"), space.toLowerCase().string());
    _tprintf(_T("'%s'\n"), space.trimLeft().string());
    _tprintf(_T("'%s'\n"), space.trimRight().string());
    _tprintf(_T("'%s'\n"), space.trim().string());
    _tprintf(_T("'%s'\n"), (space - L' ').string());

    String d = b * 7;
    d.out();

    d = b * -3;
    d.out();
}

void arrayTest() {
    int a[] = {1,2,3,4,5,6,7};
    Array<int> arr(a, 7);
    std::cout << arr << std::endl;
    std::cout << arr[1] << std::endl;

    Array<int> arr2(3);
    std::cout << arr2 << std::endl;

    Array<int> c = arr + arr2;
    std::cout << c << std::endl;

    c.reverse();
    std::cout << c << std::endl;
}

int main(int argc, const char** argv) {
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

    stringTest();
    arrayTest();

#ifdef _MSC_VER
    system("pause");
#endif

    return 0;
}
