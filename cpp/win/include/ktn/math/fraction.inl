#pragma once

/*!
 @file  math/fraction.inl
 @brief inline functions for Fraction
 */

namespace ktn { namespace math {

inline bool Fraction::operator!() const
{
    return !n_;
}

inline Fraction Fraction::operator+() const
{
    return *this;
}

inline Fraction Fraction::operator-() const
{
    Fraction n(*this);
    n.s_ = !n.s_;

    return n;
}

inline Fraction& Fraction::operator++()
{
    ++n_;
    return *this;
}

inline Fraction Fraction::operator++(int)
{
    Fraction tmp(*this);
    ++*this;
    return tmp;
}

inline Fraction& Fraction::operator--()
{
    --n_;
    return *this;
}

inline Fraction Fraction::operator--(int)
{
    Fraction tmp(*this);
    --*this;
    return tmp;
}

inline Fraction Fraction::operator+(const Fraction& b) const
{
    if (s_ == b.s_) { return Fraction(n_ * b.d_ + d_ * b.n_, d_ * b.d_, s_); }
    const ULong m(n_ * b.d_), n(d_ * b.n_);
    if (m < n) { return Fraction(n - m, d_ * b.d_, b.s_); }
    return Fraction(m - n, d_ * b.d_, s_);
}

inline Fraction Fraction::operator-(const Fraction& b) const
{
    if (s_ != b.s_) { return Fraction(n_ * b.d_ + d_ * b.n_, d_ * b.d_, s_); }
    const ULong m(n_ * b.d_), n(d_ * b.n_);
    if (m < n) { return Fraction(n - m, d_ * b.d_, !b.s_); }
    return Fraction(m - n, d_ * b.d_, s_);
}

inline Fraction Fraction::operator*(const Fraction& b) const
{
    return Fraction(n_ * b.n_, d_ * b.d_, s_ == b.s_);
}

inline Fraction Fraction::operator/(const Fraction& b) const
{
    return Fraction(n_ * b.d_, d_ * b.n_, s_ == b.s_);
}

inline Fraction Fraction::operator<<(ddigit n) const
{
    return Fraction(n_ << n, d_);
}

inline Fraction Fraction::operator>>(ddigit n) const
{
    return Fraction(n_, d_ << n);
}

inline Fraction& Fraction::operator=(const Fraction& b)
{
    if (this == &b) { return *this; }
    s_ = b.s_;
    n_ = b.n_;
    d_ = b.d_;

    return *this;
}

inline Fraction& Fraction::operator+=(const Fraction& b)
{
    *this = *this + b;
    return *this;
}

inline Fraction& Fraction::operator-=(const Fraction& b)
{
    *this = *this - b;
    return *this;
}

inline Fraction& Fraction::operator*=(const Fraction& b)
{
    *this = *this * b;
    return *this;
}

inline Fraction& Fraction::operator/=(const Fraction& b)
{
    *this = *this / b;
    return *this;
}

inline Fraction& Fraction::operator<<=(ddigit n)
{
    *this = *this << n;
    return *this;
}

inline Fraction& Fraction::operator>>=(ddigit n)
{
    *this = *this >> n;
    return *this;
}

inline int Fraction::cmp(const Fraction& b) const
{
    if (this == &b) { return 0; }
    if (s_ != b.s_) { return s_ ? 1 : -1; }
    int tmp = (n_ * b.d_).cmp(d_ * b.n_);
    return s_ ? tmp : -tmp;
}

inline bool Fraction::operator==(const Fraction& b) const
{
    return cmp(b) == 0;
}

inline bool Fraction::operator!=(const Fraction& b) const
{
    return cmp(b) != 0;
}

inline bool Fraction::operator>(const Fraction& b) const
{
    return cmp(b) > 0;
}

inline bool Fraction::operator<(const Fraction& b) const
{
    return cmp(b) < 0;
}

inline bool Fraction::operator>=(const Fraction& b) const
{
    return cmp(b) >= 0;
}

inline bool Fraction::operator<=(const Fraction& b) const
{
    return cmp(b) <= 0;
}

inline Fraction Fraction::operator+(ddigit b) const
{
    return *this + Fraction(b);
}

inline Fraction Fraction::operator-(ddigit b) const
{
    return *this - Fraction(b);
}

inline Fraction Fraction::operator*(ddigit b) const
{
    return *this * Fraction(b);
}

inline Fraction Fraction::operator/(ddigit b) const
{
    return *this / Fraction(b);
}

inline Fraction& Fraction::operator+=(ddigit b)
{
    *this = *this + b;
    return *this;
}

inline Fraction& Fraction::operator-=(ddigit b)
{
    *this = *this - b;
    return *this;
}

inline Fraction& Fraction::operator*=(ddigit b)
{
    *this = *this * b;
    return *this;
}

inline Fraction& Fraction::operator/=(ddigit b)
{
    *this = *this / b;
    return *this;
}

inline int Fraction::cmp(ddigit b) const
{
    return cmp(Fraction(b));
}

inline bool Fraction::operator==(ddigit b) const
{
    return cmp(b) == 0;
}

inline bool Fraction::operator!=(ddigit b) const
{
    return cmp(b) != 0;
}

inline bool Fraction::operator>(ddigit b) const
{
    return cmp(b) > 0;
}

inline bool Fraction::operator<(ddigit b) const
{
    return cmp(b) < 0;
}

inline bool Fraction::operator>=(ddigit b) const
{
    return cmp(b) >= 0;
}

inline bool Fraction::operator<=(ddigit b) const
{
    return cmp(b) <= 0;
}

inline Fraction::Fraction(ddigit a) :
	d_(1)
{
	if (a < 0) {
		s_ = false;
		n_ = ULong(-a);
	}
	else {
		s_ = true;
		n_ = ULong(a);
	}
}

/*!
@throw std::logic_error
*/
inline Fraction::Fraction(const ULong& a, const ULong& b, bool s) :
	s_(s)
{
	n_ = a;
	d_ = b;
	if (!d_) {
		throw std::logic_error("ZeroDivision");
	}
	cancel();
}

inline Fraction::Fraction(const char *s, int radix)
{
	// TODO:
	_ASSERT_EXPR(false, L"Not implementation");
}

inline Fraction::Fraction(const Fraction& f)
{
	if (this == &f) { return; }
	s_ = f.s_;
	n_ = f.n_;
	d_ = f.d_;
}

inline void Fraction::cancel()
{
	ULong g(n_.gcdBin(d_));
	n_ /= g;
	d_ /= g;
}

/*!
@throw std::logic_error
*/
inline Fraction::Fraction(ddigit a, ddigit b)
{
	if (a < 0) {
		s_ = false;
		n_ = ULong(-a);
	}
	else {
		s_ = true;
		n_ = ULong(a);
	}
	if (b == 0) {
		throw std::logic_error("ZeroDivision");
	}
	else if (b < 0) {
		s_ = !s_;
		d_ = ULong(-b);
	}
	else {
		d_ = ULong(b);
	}

	cancel();
}

inline std::string Fraction::str(int radix) const
{
	std::string s("");
	if (!s_) { s = "-"; }
	s += n_.str(radix);
	s += "/";
	s += d_.str(radix);

	return s;
}

inline std::ostream& operator<<(std::ostream& os, const Fraction& f)
{
	return os << f.str(10);
}

inline std::istream& operator>>(std::istream& is, Fraction& f)
{
	std::string str;
	is >> str;
	f = Fraction(str.c_str(), 10);
	return is;
}

inline void Fraction::out(int radix, bool br) const
{
	if (!s_) { putchar('-'); }
	n_.out(radix, false);
	putchar('/');
	d_.out(radix, false);
	if (br) { puts(""); }
}

}} // namespace ktn math
