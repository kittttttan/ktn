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

using namespace ktn;

void stringTest() {
    Logging logging;
    String title("Samples for String class");
    LOGGING_INFO(logging, title);

    String a("Nana"), b("Mizuki");
    printf("%s: %d\n", a.string(), a.length());
    printf("%c\n", a[-1]);

    String c = a + b;
    c.out();
    (-c).out();
    std::cout << c << std::endl;

    printf("%d\n", c.indexOf('i'));
    printf("%d\n", c.lastIndexOf('a', 2));
    printf("%s\n", c.slice(2).string());
    printf("%s\n", c.slice(0, -3).string());
    printf("%s\n", c.substr(2, 2).string());

    String space("  Nana  Mizuki   ");
    printf("'%s'\n", space.toUpperCase().string());
    printf("'%s'\n", space.toLowerCase().string());
    printf("'%s'\n", space.trimLeft().string());
    printf("'%s'\n", space.trimRight().string());
    printf("'%s'\n", space.trim().string());
    printf("'%s'\n", (space - ' ').string());

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
    stringTest();
    arrayTest();

    return 0;
}
