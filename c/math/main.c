#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

int main() {
  Long u;
  int n;

#ifdef _MSC_VER
  clock_t t = clock();

  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

  basic(7777777, -10000);

  longInit(&u);

  n = 777;
  printf("fib(%d) =\n", n); fib(&u, n);
  longWriteln(&u);

  n = 77;
  printf("fact(%d) =\n", n); fact(&u, n);
  longWriteln(&u);

  // FIXME: memory leak
  //printf("pi(%d) =\n", n); pi(&u, n);
  //longWriteln(&u);

  longFree(&u);

#ifdef _MSC_VER
  printf("%ld ms\n", clock() - t);
  system("pause");
#endif

  return 0;
}
