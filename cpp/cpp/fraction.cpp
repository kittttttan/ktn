/**
 * fraction.cpp - Fraction
 */
#include <cstdio>

#ifndef _MSC_VER
#include <cstdlib>
#include <cstring>
#endif

#include "fraction.h"

namespace mathktn {

static const Fraction ZERO(0);
static const Fraction ONE(1);

Fraction::Fraction() : s_(true), n_(0), d_(1) {

}

Fraction::Fraction(int64_t a) : d_(1) {
	if (a < 0) {
		s_ = false;
		n_ = ULong(-a);
	} else {
		s_ = true;
		n_ = ULong(a);
	}
}

Fraction::Fraction(const ULong& a, bool s) : d_(1), s_(s) {
	n_ = a;
}

Fraction::Fraction(const ULong& a, const ULong& b, bool s) : s_(s) {
	n_ = a;
	d_ = b;
	if (!d_) { fprintf(stderr, "ZeroDivision: Fraction"); d_ = ULong(1); }
	cancel();
}

Fraction::Fraction(const Fraction& f) {
	if (this == &f) { return; }
	s_ = f.s_;
	n_ = f.n_;
	d_ = f.d_;
}

Fraction::~Fraction() {

}

inline void Fraction::cancel() {
	ULong g(n_.gcdBin(d_));
	n_ /= g;
	d_ /= g;
}

Fraction::Fraction(int64_t a, int64_t b) {
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

void Fraction::out(int base, bool br) {
	if (!s_) { putchar('-'); }
	n_.out(base, false);
	putchar('/');
	d_.out(base, false);
	if (br) { puts(""); }
}

bool Fraction::operator!() const {
	return !n_;
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
	return Fraction(n_ * b.d_ + d_ * b.n_, d_ * b.d_);
}

Fraction Fraction::operator-(const Fraction& b) const {
	return Fraction(n_ * b.d_ - d_ * b.n_, d_ * b.d_);
}

Fraction Fraction::operator*(const Fraction& b) const {
	return Fraction(n_ * b.n_, d_ * b.d_);
}

Fraction Fraction::operator/(const Fraction& b) const {
	return Fraction(n_ * b.d_, d_ * b.n_);
}

Fraction Fraction::operator<<(int64_t n) const {
	return Fraction(n_ << n, d_);
}

Fraction Fraction::operator>>(int64_t n) const {
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

Fraction& Fraction::operator<<=(int64_t n) {
	*this = *this << n;
	return *this;
}

Fraction& Fraction::operator>>=(int64_t n) {
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

} // namespace mathktn