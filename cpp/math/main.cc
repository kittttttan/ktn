#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "test.h"

int main(int argc, char** argv) {
  srand(static_cast<unsigned int>(time(NULL)));

#if defined(_MSC_VER)
  clock_t t = clock();
#endif

  ulongBasic();
  longBasic();
  fractionBasic();
  compBasic();
  vectorBasic();
  matrixBasic();

  //fib<ktn::math::ULong>(7777).out();
  //fact<ktn::math::ULong>(777).out();
  //pi(777).out();
  //kTest();

#if defined(_MSC_VER)
  printf("%ld ms\n", clock() - t);
  system("pause");
#endif
  return 0;
}
