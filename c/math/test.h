#ifndef CKTN_TEST_H_
#define CKTN_TEST_H_

#include "ulong.h"

#define UNIT_TEST

#ifdef __cplusplus
extern "C" {
#endif

void basic(ddigit m, ddigit n);
void fib(ULong* dest, int n);
void fact(ULong* dest, int n);
void arccot(ULong* dest, ULong* n, int m);
void pi(ULong* dest, ddigit a);

#ifdef __cplusplus
}
#endif

#endif // CKTN_TEST_H_