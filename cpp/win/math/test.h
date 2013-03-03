#pragma once

#include "long.h"
#include "fraction.h"
#include "math.h"

template <typename Integer>
Integer fib(int n)
{
    Integer a(1), b(0);
    for (int i = 1; i < n; ++i) {
        const Integer c(a);
        a += b;
        b = c;
    }

    return a;
}

template <typename Integer>
Integer fact(int n)
{
    Integer a(1);
    for (int i = 2; i <= n; ++i) {
        a *= i;
    }

    return a;
}

void ulongBasic();
void longBasic();
void fractionBasic();
ktn::math::ULong arccot(const ktn::math::ULong& n, int m);
ktn::math::ULong pi(ktn::math::ddigit a);
void kTest();
void compBasic();
void vectorBasic();
void matrixBasic();
