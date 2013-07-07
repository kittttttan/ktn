#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "test.h"

int main(int argc, char** argv)
{
    clock_t t = clock();

    ulongBasic();
    longBasic();
    fractionBasic();
    compBasic();
    vectorBasic();
    matrixBasic();

    //fib<ktn::math::ULong>(10000).out();
    //fact<ktn::math::ULong>(777).out();
    //ktn::math::ULong::factorial(777).out();
    //pi(777).out();
    //kTest();

    ::printf("%ld ms\n", clock() - t);
    ::system("pause");

    return 0;
}
