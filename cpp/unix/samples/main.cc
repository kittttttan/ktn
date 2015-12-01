#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "math-test.h"

int main(int argc, char** argv)
{
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

    return 0;
}
