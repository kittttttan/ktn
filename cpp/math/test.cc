#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "long.h"
#include "fraction.h"
#include "math.h"
#include "complex.h"

using namespace std;
using namespace ktn::math;

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
	printf("b + 7 = "); (b + 7).out();
	printf("b - 7 = "); (b - 7).out();
	printf("b * 7 = "); (b * 7).out();
	printf("b / 7 = "); (b / 7).out();
	printf("b %% 7 = "); (b % 7).out();
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
}

void fractionBasic() {
	Fraction a(154, 6), b(-11, 5);

	puts("** Test for Fraction **");
	printf("a = "); a.out();
	printf("b = "); b.out();
	printf("a + b = "); (a + b).out();
	printf("a - b = "); (a - b).out();
	printf("a * b = "); (a * b).out();
	printf("a / b = "); (a / b).out();
}

template <typename Integer>
Integer fib(int n) {
	Integer a(1), b(0);
	for (int i = 1; i < n; ++i) {
		const Integer c(a);
		a += b;
		b = c;
	}

	return a;
}

template <typename Integer>
Integer fact(int n) {
	Integer a(1);
	for (int i = 2; i <= n; ++i) {
		a *= i;
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

	while (c != 0) {
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
}

ULong pi(BitSize a) {
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

void compBasic() {
	Complex<double> a(1,2), b(3,-4);
	puts("** Complex<double> **");
	printf("a = "); a.out();
	printf("b = "); b.out();
	printf("a + b = "); (a+b).out();
	printf("a - b = "); (a-b).out();
	printf("a * b = "); (a*b).out();
	printf("a / b = "); (a/b).out();

	Complex<Long> c(Long(10000), Long(77777)), d(Long(14235),Long(-4156123));
	puts("** Complex<Long> **");
	printf("c = "); c.out();
	printf("d = "); d.out();
	printf("c + d = "); (c+d).out();
	printf("c - d = "); (c-d).out();
	printf("c * d = "); (c*d).out();
}

int main(int argc, char** argv) {
	srand(static_cast<unsigned int>(time(NULL)));
	clock_t t = clock();

	ulongBasic();
	longBasic();
	fractionBasic();
	compBasic();

	//fib<ULong>(77).out();
	//fact<ULong>(77).out();
	//pi(2000).out();
	//kTest();

	printf("%ld ms\n", clock() - t);
#if defined(_MSC_VER)
	system("pause");
#endif
	return 0;
}