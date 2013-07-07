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

}} // namespace ktn math
