#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "long.h"
#include "fraction.h"

using namespace std;
using namespace mathktn;

void ulongBasic() {
	ULong a = ULong::random(4), b(100000);

	puts("** Test for ULong **");
	printf("a = "); a.out();
	printf("b = "); b.out();
	printf("!a = %s\n", !a ? "true" : "false");
	printf("a < b is %s\n", (a < b) ? "true" : "false");
	printf("a + b = "); (a + b).out();
	printf("a - b = "); (a - b).out();
	printf("a * b = "); (a * b).out();
	printf("a / b = "); (a / b).out();
	printf("a %% b = "); (a % b).out();
	printf("b >> 7 = "); (b >> 7).out();
	printf("b << 7 = "); (b << 7).out();
	printf("b ** 2 = "); b.square().out();
	printf("b ** .5 = "); b.sqrt().out();
	printf("b ** 10 = "); b.pow(10).out();
	printf("GCD(a, b) = "); a.gcd(b).out();
	printf("          = "); a.gcdBin(b).out();
	printf("++a = "); (++a).out();
}

void longBasic() {
	Long a("-1234567890987654321", 10), b(10000000);

	puts("** Test for Long **");
	printf("a = "); a.out();
	printf("b = "); b.out();
	printf("!a = %s\n", !a ? "true" : "false");
	printf("-a = "); (-a).out();
	printf("a < b is %s\n", (a < b) ? "true" : "false");
	printf("a + b = "); (a + b).out();
	printf("a - b = "); (a - b).out();
	printf("a * b = "); (a * b).out();
	printf("a / b = "); (a / b).out();
	printf("a %% b = "); (a % b).out();
	printf("++a = "); (++a).out();
	printf("a-- = "); (a--).out();
}

void fractionBasic() {
	Fraction a(154, 6), b(11, 5);

	puts("** Test for Fraction **");
	printf("a = "); a.out();
	printf("b = "); b.out();
	printf("a + b = "); (a + b).out();
	printf("a - b = "); (a - b).out();
	printf("a * b = "); (a * b).out();
	printf("a / b = "); (a / b).out();
}

ULong fib(int n) {
	ULong a(1), b(0), c;
	for (int i = 1; i < n; ++i) {
		c = a;
		//a = a + b;
		a += b;
		b = c;
	}

	return a;
}

ULong fact(int64_t n) {
	ULong a(1);
	for (int64_t i = 2; i <= n; ++i) {
		a *= ULong(i);
	}

	return a;
}

ULong arccot(const ULong& n, int m) {
	ULong c(n);
	ULong m2(m);
	ULong a(c / m2);
	ULong b(a);
	ULong k(1), l2(2);
	bool s = true;
	m2 = m2.square();

	while (!!c) {
		b /= m2;
		k += l2;
		c = b / k;
		s = !s;
		if (s) {
			a += c;
		} else {
			a -= c;
		}
	}

	return a;
};

ULong pi(int64_t a) {
	ULong n(10);
	n = n.pow(a);

	return ((arccot(n, 5) << 2) - arccot(n, 239)) << 2;
}

void kTest() {
	ULong a = ULong::random(1100), b = ULong::random(1100), c, d;
	clock_t t = clock(), t1, t2;
	c = a * b;
	t1 = clock();
	d = a.karatsuba(b);
	t2 = clock();
	//c.out();
	//d.out();
	printf("%d\nk: %ldms\n*: %ldms\n", c == d, t2 - t1, t1 - t);
}

int main(int argc, char** argv) {
	clock_t t = clock();

	srand(static_cast<unsigned int>(time(NULL)));

	//ulongBasic();
	//longBasic();
	fractionBasic();

	//fib(77).out();
	//fact(77).out();
	//pi(200).out();
	//kTest();

	printf("%ld ms\n", clock() - t);
#if defined(_MSC_VER) && defined(_DEBUG)
	system("pause");
#endif
	return 0;
}