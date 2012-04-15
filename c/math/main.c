#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
#if defined(_MSC_VER)
	clock_t t = clock();
#endif // defined(_MSC_VER)

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

#if defined(_MSC_VER)
	printf("%ld ms\n", clock() - t);
	system("pause");
#endif // defined(_MSC_VER)

	return 0;
}
