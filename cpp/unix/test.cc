/**
 * @file  cpp/test.cc
 * @brief test for String
 */
#include "dbg.h"
#include "loggingw.h"
#include "stringw.h"
#include "array.h"

#include <cstdio>
#include <cstdlib>
#include <clocale>

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

void arrayTest() {
    int a[] = {1,2,3,4,5,6,7};
    Array<int> arr(a, 7);
    std::wcout << arr << std::endl;
    std::wcout << arr[1] << std::endl;

    Array<int> arr2(3);
    std::wcout << arr2 << std::endl;

    Array<int> c = arr + arr2;
    std::wcout << c << std::endl;

    c.reverse();
    std::wcout << c << std::endl;
}

int main(int argc, const char** argv) {
    stringTest();
    arrayTest();

    return 0;
}
