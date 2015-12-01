#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "math-test.h"

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    clock_t t = clock();

    bool basic = true;
    if (basic) {
        ulongBasic();
        longBasic();
        fractionBasic();
        compBasic();
        vectorBasic();
        matrixBasic();
    }

    bool out = false;
    if (out) {
        fib<ktn::math::ULong>(25000).out();
        ktn::math::ULong::factorial(5000).out();
        pi(777).out();
    } else {
        fib<ktn::math::ULong>(25000);
        ktn::math::ULong::factorial(5000);
        pi(777);
    }

    //kTest();

    ::printf("%ld ms\n", clock() - t);
    ::system("pause");

    return 0;
}
