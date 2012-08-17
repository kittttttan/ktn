/**
 * @file  cpp/test.cc
 * @brief test for String
 */
#include "dbg.h"
#include "logging.h"
#include "loggingw.h"
#include "string.h"
#include "stringw.h"
#include "array.h"

#include <cstdio>
#include <cstdlib>
#include <clocale>

using namespace ktn;

void stringTest() {
    setlocale(LC_CTYPE, "");

    Logging logging;
    String title("Samples for String class");
    LOGGING_INFO(logging, title);

    StringW a(L"水樹"), b(L"奈々");
    wprintf(L"%s: %d\n", a.string(), a.length());
    wprintf(L"%c\n", a[-1]);

    StringW c = a + b;
    c.out();
    (-c).out();
    std::wcout << c << std::endl;

    wprintf(L"%d\n", c.indexOf(L'々'));
    wprintf(L"%d\n", c.lastIndexOf(L'水', 2));
    wprintf(L"%s\n", c.slice(2).string());
    wprintf(L"%s\n", c.slice(0, -3).string());
    wprintf(L"%s\n", c.substr(2, 2).string());

    String space("  Nana  Mizuki   ");
    printf("'%s'\n", space.toUpperCase().string());
    printf("'%s'\n", space.toLowerCase().string());
    printf("'%s'\n", space.trimLeft().string());
    printf("'%s'\n", space.trimRight().string());
    printf("'%s'\n", space.trim().string());
    printf("'%s'\n", (space - ' ').string());

    StringW d = b * 7;
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
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    stringTest();
    arrayTest();

    system("pause");

    return 0;
}
