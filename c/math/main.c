#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
#if defined(_MSC_VER)
	clock_t t = clock();
#endif

	ULong u;
	int n;

	basic();

	ulongInit(&u);

	n = 777;
	printf("fib(%d) =\n", n); fib(&u, n);
	ulongWriteln(&u);

	n = 77;
	printf("fact(%d) =\n", n); fact(&u, n);
	ulongWriteln(&u);

	printf("pi(%d) =\n", n); pi(&u, n);
	ulongWriteln(&u);

	ulongFree(&u);

#if defined(_MSC_VER)
	printf("%ld ms\n", clock() - t);
	system("pause");
#endif

	return 0;
}