#include "test.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void basic(ddigit m, ddigit n) {
	ULong a, b, t;
	ulongInit(&a);
	ulongInit(&b);
	ulongInit(&t);

	ulongNum(&a, m);
	ulongNum(&b, n);

	printf("a = "); ulongWriteln(&a);
	printf("b = "); ulongWriteln(&b);

	ulongAdd(&t, &a, &b);
	printf("a + b = "); ulongWriteln(&t);

	ulongSub(&t, &a, &b);
	printf("a - b = "); ulongWriteln(&t);

	ulongMul(&t, &a, &b);
	printf("a * b = "); ulongWriteln(&t);

	ulongDiv(&t, &a, &b);
	printf("a / b = "); ulongWriteln(&t);

	ulongMod(&t, &a, &b);
	printf("a %% b = "); ulongWriteln(&t);

	ulongSquare(&t, &b);
	printf("b ^ 2 = "); ulongWriteln(&t);

	ulongSqrt(&t, &b);
	printf("b ^ .5 = "); ulongWriteln(&t);

	ulongPow(&t, &b, 7);
	printf("b ^ 7 = "); ulongWriteln(&t);

	ulongLeftShift(&t, &b, 7);
	printf("b << 7 = "); ulongWriteln(&t);

	ulongFree(&a);
	ulongFree(&b);
	ulongFree(&t);
}

void fib(ULong* dest, int n) {
	int i;
	ULong a, b, ab;
	ulongInit(&a);
	ulongInit(&b);
	ulongInit(&ab);

	ulongNum(&a, 1);
	ulongNum(&b, 0);
	for (i = 1; i < n; ++i) {
		ulongAdd(&ab, &a, &b);
		ulongClone(&b, &a);
		ulongClone(&a, &ab);
	}
	ulongClone(dest, &a);

	ulongFree(&a);
	ulongFree(&b);
	ulongFree(&ab);
}

void fact(ULong* dest, int n) {
	int i;
	ULong a, t;
	ulongInit(&a);
	ulongInit(&t);

	ulongNum(&a, 1);
	for (i = 2; i <= n; ++i) {
		ulongNum(&t, i);
		ulongMul(dest, &a, &t);
		ulongClone(&a, dest);
	}

	ulongFree(&a);
	ulongFree(&t);
}

void arccot(ULong* dest, ULong* n, int m) {
	ULong c, m2, a, b, k, l2, t;
	bool s = true;

	ulongInit(&c);
	ulongInit(&m2);
	ulongInit(&a);
	ulongInit(&b);
	ulongInit(&k);
	ulongInit(&l2);
	ulongInit(&t);

	ulongClone(&c, n);
	ulongNum(&t, m);
	ulongDiv(&a, &c, &t);
	ulongClone(&b, &a);
	ulongNum(&k, 1);
	ulongNum(&l2, 2);
	ulongSquare(&m2, &t);

	while (!ulongIsZero((&c))) {
		ulongDiv(&t, &b, &m2);
		ulongClone(&b, &t);

		ulongAdd(&t, &k, &l2);
		ulongClone(&k, &t);

		ulongDiv(&c, &b, &k);
		s = !s;
		if (s) {
			ulongAdd(&t, &a, &c);
		} else {
			ulongSub(&t, &a, &c);
		}
		ulongClone(&a, &t);
	}

	ulongClone(dest, &a);

	ulongFree(&c);
	ulongFree(&m2);
	ulongFree(&a);
	ulongFree(&b);
	ulongFree(&k);
	ulongFree(&l2);
	ulongFree(&t);
}

void pi(ULong* dest, ddigit a) {
	ULong n, t, a5, a239;
	ulongInit(&n);
	ulongInit(&t);
	ulongInit(&a5);
	ulongInit(&a239);
	ulongNum(&t, 10);
	ulongPow(&n, &t, a);

	arccot(&a5, &n, 5);
	ulongLeftShift(dest, &a5, 2);

	arccot(&a239, &n, 239);

	ulongSub(&t, dest, &a239);

	ulongLeftShift(dest, &t, 2);
	
	ulongFree(&n);
	ulongFree(&t);
	ulongFree(&a5);
	ulongFree(&a239);
}

#ifdef __cplusplus
}
#endif
