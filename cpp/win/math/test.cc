/**
 * @file  math/test.cc
 * @brief test for ktn::math
 * @date  2012
 */

#include "test.h"
#include "complex.h"
#include "vector.h"
#include "matrix.h"

#include <cstdio>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace ktn::math;

void ulongBasic()
{
    ULong a = ULong::random(4), b(100000);

    puts("** Test for ULong **");
    printf("a = "); a.out();
    // FIXME: 
    cout << "  = " << hex << a << endl
         << "  = " << a << endl;
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

void longBasic()
{
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

void fractionBasic()
{
    Fraction a(154, 6), b(-11, 5);

    puts("** Test for Fraction **");
    printf("a = "); a.out();
    printf("b = "); b.out();
    printf("a + b = "); (a + b).out();
    printf("a - b = "); (a - b).out();
    printf("a * b = "); (a * b).out();
    printf("a / b = "); (a / b).out();
}

ULong arccot(const ULong& n, int m)
{
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

ULong pi(ddigit a)
{
    ULong n(10);
    n = n.pow(a);

    return ((arccot(n, 5) << 2) - arccot(n, 239)) << 2;
}

void kTest()
{
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

void compBasic()
{
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

void vectorBasic()
{
    Vector<int, 3> a, b;
    cout << "** " << a.dimension() << "D Vector **" << endl;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    cout << a << endl;
    cout << (-a) << endl;
    cout << (a*2) << endl;

    b[1] = 3;
    b[2] = 4;
    cout << b << endl;
    cout << (a+b) << endl;
    cout << (a-b) << endl;
    cout << (a*b) << endl;
}

void matrixBasic()
{
    Matrix<int,3,2> n;
    cout << "** " <<  n.row() << "x"<< n.col() << " Matrix **" << endl;
    n.at(1,1) = 1;
    n.at(3,2) = 2;
    cout << n << endl;
    cout << n.t() << endl;

    Matrix<double, 3> m, k;
    double d = 3;
    m.at(1,1) = 1;
    m.at(2,2) = 2;
    m.at(3,3) = 3;
    cout << m << endl;
    //cout << m.cof(1, 1) << endl;

    cout << (m*d) << endl;
    //cout << m.det() << endl;
}
