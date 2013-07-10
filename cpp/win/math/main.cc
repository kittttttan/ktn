#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "test.h"

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    clock_t t = clock();

    //ulongBasic();
    //longBasic();
    //fractionBasic();
    //compBasic();
    //vectorBasic();
    //matrixBasic();

    fib<ktn::math::ULong>(25000).out();
    //fact<ktn::math::ULong>(777).out();
    ktn::math::ULong::factorial(5000).out();
    //pi(777).out();
    //kTest();

    ::printf("%ld ms\n", clock() - t);
    ::system("pause");

    return 0;
}
