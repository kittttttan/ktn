#include "test.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void basic(ddigit m, ddigit n) {
  Long a, b, t;
  longInit(&a);
  longInit(&b);
  longInit(&t);

  longNum(&a, m);
  longNum(&b, n);

  printf("a = "); longWriteln(&a);
  printf("b = "); longWriteln(&b);

  longAdd(&t, &a, &b);
  printf("a + b = "); longWriteln(&t);

  longSub(&t, &a, &b);
  printf("a - b = "); longWriteln(&t);

  longMul(&t, &a, &b);
  printf("a * b = "); longWriteln(&t);

  longDiv(&t, &a, &b);
  printf("a / b = "); longWriteln(&t);

  longMod(&t, &a, &b);
  printf("a %% b = "); longWriteln(&t);

  longSquare(&t, &b);
  printf("b ^ 2 = "); longWriteln(&t);

  longSqrt(&t, &b);
  printf("b ^ .5 = "); longWriteln(&t);

  longPow(&t, &b, 7);
  printf("b ^ 7 = "); longWriteln(&t);

  longLeftShift(&t, &b, 7);
  printf("b << 7 = "); longWriteln(&t);

  longFree(&a);
  longFree(&b);
  longFree(&t);
}

void fib(Long* dest, int n) {
  int i;
  Long a, b, ab;
  longInit(&a);
  longInit(&b);
  longInit(&ab);

  longNum(&a, 1);
  longNum(&b, 0);
  for (i = 1; i < n; ++i) {
    longAdd(&ab, &a, &b);
    longClone(&b, &a);
    longClone(&a, &ab);
  }
  longClone(dest, &a);

  longFree(&a);
  longFree(&b);
  longFree(&ab);
}

void fact(Long* dest, int n) {
  int i;
  Long a, t;
  longInit(&a);
  longInit(&t);

  longNum(&a, 1);
  for (i = 2; i <= n; ++i) {
    longNum(&t, i);
    longMul(dest, &a, &t);
    longClone(&a, dest);
  }

  longFree(&a);
  longFree(&t);
}

// FIXME: memory leak
void arccot(Long* dest, Long* n, int m) {
  Long c, m2, a, b, k, l2, t;
  bool s = true;

  longInit(&c);
  longInit(&m2);
  longInit(&a);
  longInit(&b);
  longInit(&k);
  longInit(&l2);
  longInit(&t);

  longClone(&c, n);
  longNum(&t, m);
  longDiv(&a, &c, &t);
  longClone(&b, &a);
  longNum(&k, 1);
  longNum(&l2, 2);
  longSquare(&m2, &t);

  while (!longIsZero((&c))) {
    longDiv(&t, &b, &m2);
    longClone(&b, &t);

    longAdd(&t, &k, &l2);
    longClone(&k, &t);

    longDiv(&c, &b, &k);
    s = !s;
    if (s) {
      longAdd(&t, &a, &c);
    } else {
      longSub(&t, &a, &c);
    }
    longClone(&a, &t);
  }

  longClone(dest, &a);

  longFree(&c);
  longFree(&m2);
  longFree(&a);
  longFree(&b);
  longFree(&k);
  longFree(&l2);
  longFree(&t);
}

void pi(Long* dest, ddigit a) {
  Long n, t, a5, a239;
  longInit(&n);
  longInit(&t);
  longInit(&a5);
  longInit(&a239);
  longNum(&t, 10);
  longPow(&n, &t, a);

  arccot(&a5, &n, 5);
  longLeftShift(dest, &a5, 2);

  arccot(&a239, &n, 239);

  longSub(&t, dest, &a239);

  longLeftShift(dest, &t, 2);
  
  longFree(&n);
  longFree(&t);
  longFree(&a5);
  longFree(&a239);
}

#ifdef __cplusplus
}
#endif
