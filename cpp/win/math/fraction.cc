/**
 * @file  math/fraction.cc
 * @brief Fraction
 */
#include <cstdio>

#include "fraction.h"

namespace ktn { namespace math {

const Fraction Fraction::ZERO(0);
const Fraction Fraction::ONE(1);

Fraction::Fraction(ddigit a) : d_(1) {
    if (a < 0) {
        s_ = false;
        n_ = ULong(-a);
    } else {
        s_ = true;
        n_ = ULong(a);
    }
}

Fraction::Fraction(const ULong& a, const ULong& b, bool s) : s_(s) {
    n_ = a;
    d_ = b;
    if (!d_) { fprintf(stderr, "ZeroDivision: Fraction"); d_ = ULong(1); return; }
    cancel();
}

Fraction::Fraction(const Fraction& f) {
    if (this == &f) { return; }
    s_ = f.s_;
    n_ = f.n_;
    d_ = f.d_;
}

inline void Fraction::cancel() {
    ULong g(n_.gcdBin(d_));
    n_ /= g;
    d_ /= g;
}

Fraction::Fraction(ddigit a, ddigit b) {
    if (a < 0) {
        s_ = false;
        n_ = ULong(-a);
    } else {
        s_ = true;
        n_ = ULong(a);
    }
    if (b == 0) {
        fprintf(stderr, "ZeroDivision: Fraction");
        d_ = ULong(1);
    } else if (b < 0) {
        s_ = !s_;
        d_ = ULong(-b);
    } else {
        d_ = ULong(b);
    }

    cancel();
}

std::string Fraction::str(int radix) const {
    std::string s("");
    if (!s_) { s = "-"; }
    s += n_.str(radix);
    s += "/";
    s += d_.str(radix);

    return s;
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.str(10);
}
/*
std::istream& operator>>(std::istream& is, Fraction& f) {
return is;
}
*/

void Fraction::out(int radix, bool br) const {
    if (!s_) { putchar('-'); }
    n_.out(radix, false);
    putchar('/');
    d_.out(radix, false);
    if (br) { puts(""); }
}

bool Fraction::operator!() const {
    return !n_;
}

Fraction Fraction::operator+() const {
    return *this;
}

Fraction Fraction::operator-() const {
    Fraction n(*this);
    n.s_ = !n.s_;

    return n;
}

Fraction& Fraction::operator++() {
    ++n_;
    return *this;
}

Fraction Fraction::operator++(int) {
    Fraction tmp(*this);
    ++*this;
    return tmp;
}

Fraction& Fraction::operator--() {
    --n_;
    return *this;
}

Fraction Fraction::operator--(int) {
    Fraction tmp(*this);
    --*this;
    return tmp;
}

Fraction Fraction::operator+(const Fraction& b) const {
    if (s_ == b.s_) { return Fraction(n_ * b.d_ + d_ * b.n_, d_ * b.d_, s_); }
    const ULong m(n_ * b.d_), n(d_ * b.n_);
    if (m < n) { return Fraction(n - m, d_ * b.d_, b.s_); }
    return Fraction(m - n, d_ * b.d_, s_);
}

Fraction Fraction::operator-(const Fraction& b) const {
    if (s_ != b.s_) { return Fraction(n_ * b.d_ + d_ * b.n_, d_ * b.d_, s_); }
    const ULong m(n_ * b.d_), n(d_ * b.n_);
    if (m < n) { return Fraction(n - m, d_ * b.d_, !b.s_); }
    return Fraction(m - n, d_ * b.d_, s_);
}

Fraction Fraction::operator*(const Fraction& b) const {
    return Fraction(n_ * b.n_, d_ * b.d_, s_ == b.s_);
}

Fraction Fraction::operator/(const Fraction& b) const {
    return Fraction(n_ * b.d_, d_ * b.n_, s_ == b.s_);
}

Fraction Fraction::operator<<(ddigit n) const {
    return Fraction(n_ << n, d_);
}

Fraction Fraction::operator>>(ddigit n) const {
    return Fraction(n_, d_ << n);
}

Fraction& Fraction::operator=(const Fraction& b) {
    if (this == &b) { return *this; }
    s_ = b.s_;
    n_ = b.n_;
    d_ = b.d_;

    return *this;
}

Fraction& Fraction::operator+=(const Fraction& b) {
    *this = *this + b;
    return *this;
}

Fraction& Fraction::operator-=(const Fraction& b) {
    *this = *this - b;
    return *this;
}

Fraction& Fraction::operator*=(const Fraction& b) {
    *this = *this * b;
    return *this;
}

Fraction& Fraction::operator/=(const Fraction& b) {
    *this = *this / b;
    return *this;
}

Fraction& Fraction::operator<<=(ddigit n) {
    *this = *this << n;
    return *this;
}

Fraction& Fraction::operator>>=(ddigit n) {
    *this = *this >> n;
    return *this;
}

int Fraction::cmp(const Fraction& b) const {
    if (this == &b) { return 0; }
    if (s_ != b.s_) { return s_ ? 1 : -1; }
    int tmp = (n_ * b.d_).cmp(d_ * b.n_);
    return s_ ? tmp : -tmp;
}

bool Fraction::operator==(const Fraction& b) const {
    return cmp(b) == 0;
}

bool Fraction::operator!=(const Fraction& b) const {
    return cmp(b) != 0;
}

bool Fraction::operator>(const Fraction& b) const {
    return cmp(b) > 0;
}

bool Fraction::operator<(const Fraction& b) const {
    return cmp(b) < 0;
}

bool Fraction::operator>=(const Fraction& b) const {
    return cmp(b) >= 0;
}

bool Fraction::operator<=(const Fraction& b) const {
    return cmp(b) <= 0;
}


Fraction Fraction::operator+(ddigit b) const {
    return *this + Fraction(b);
}

Fraction Fraction::operator-(ddigit b) const {
    return *this - Fraction(b);
}

Fraction Fraction::operator*(ddigit b) const {
    return *this * Fraction(b);
}

Fraction Fraction::operator/(ddigit b) const {
    return *this / Fraction(b);
}

Fraction& Fraction::operator+=(ddigit b) {
    *this = *this + b;
    return *this;
}

Fraction& Fraction::operator-=(ddigit b) {
    *this = *this - b;
    return *this;
}

Fraction& Fraction::operator*=(ddigit b) {
    *this = *this * b;
    return *this;
}

Fraction& Fraction::operator/=(ddigit b) {
    *this = *this / b;
    return *this;
}

int Fraction::cmp(ddigit b) const {
    return cmp(Fraction(b));
}

bool Fraction::operator==(ddigit b) const {
    return cmp(b) == 0;
}

bool Fraction::operator!=(ddigit b) const {
    return cmp(b) != 0;
}

bool Fraction::operator>(ddigit b) const {
    return cmp(b) > 0;
}

bool Fraction::operator<(ddigit b) const {
    return cmp(b) < 0;
}

bool Fraction::operator>=(ddigit b) const {
    return cmp(b) >= 0;
}

bool Fraction::operator<=(ddigit b) const {
    return cmp(b) <= 0;
}

}} // namespace ktn math