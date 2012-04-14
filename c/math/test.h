#ifndef CKTN_TEST_H_
#define CKTN_TEST_H_

#include "long.h"

#define UNIT_TEST

#ifdef __cplusplus
extern "C" {
#endif

void basic(ddigit m, ddigit n);
void fib(Long* dest, int n);
void fact(Long* dest, int n);
void arccot(Long* dest, Long* n, int m);
void pi(Long* dest, ddigit a);

#ifdef __cplusplus
}
#endif

#endif // CKTN_TEST_H_