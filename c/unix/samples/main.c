#include "math-test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  Long u;
  int n;

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

  return 0;
}
