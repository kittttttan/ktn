#include "math-test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)

int main() {
  Long u;
  int n;

  clock_t t = clock();

  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

  basic(7777777, -10000);

  longInit(&u);

  n = 777;
  printf("fib(%d) =\n", n); fib(&u, n);
  longWriteln(&u);

  n = 77;
  printf("fact(%d) =\n", n); fact(&u, n);
  longWriteln(&u);

  printf("pi(%d) =\n", n); pi(&u, n);
  longWriteln(&u);

  longFree(&u);

  printf("%ld ms\n", clock() - t);
  system("pause");

  return 0;
}
