#pragma once

/*!
 @file  math/long.inl
 @brief inline functions for Long
 */

namespace ktn { namespace math {

inline bool Long::operator!() const
{
    return !u_;
}

inline Long Long::operator+() const
{
    return *this;
}

inline Long Long::operator-() const
{
    Long n(*this);
    n.s_ = !n.s_;

    return n;
}

inline Long& Long::operator++()
{
    *this = *this + ONE;
    return *this;
}

inline Long Long::operator++(int)
{
    Long tmp(*this);
    ++*this;
    return tmp;
}

inline Long& Long::operator--()
{
    *this = *this - ONE;
    return *this;
}

inline Long Long::operator--(int)
{
    Long tmp(*this);
    --*this;
    return tmp;
}

inline Long& Long::operator=(const Long& b)
{
    if (this == &b) { return *this; }
    s_ = b.s_;
    u_ = b.u_;

    return *this;
}
    
inline int Long::cmp(const Long& b) const
{
    if (this == &b) { return 0; }
    if (s_ != b.s_) { return s_ ? 1 : -1; }
    return s_ ? u_.cmp(b.u_) : -u_.cmp(b.u_);
}

inline Long Long::operator+(const Long& a) const
{
    if (s_ == a.s_) {
        return Long(u_ + a.u_, s_);
    }
    if (u_ < a.u_) {
        return Long(a.u_ - u_, a.s_);
    }
    return Long(u_ - a.u_, s_);
}

inline Long Long::operator-(const Long& a) const
{
    if (s_ != a.s_) {
        return Long(u_ + a.u_, s_);
    }
    if (u_ < a.u_) {
        return Long(a.u_ - u_, !a.s_);
    }
    return Long(u_ - a.u_, s_);
}

inline Long Long::operator*(const Long& a) const
{
    return Long(u_ * a.u_, s_ == a.s_);
}

inline Long Long::operator/(const Long& a) const
{
    return Long(u_ / a.u_, s_ == a.s_);
}

inline Long Long::operator%(const Long& a) const
{
    return Long(u_ % a.u_, s_);
}

inline Long Long::operator<<(ddigit n) const
{
    return Long(u_ << n, true);
}

inline Long Long::operator>>(ddigit n) const
{
    return Long(u_ >> n, true);
}

inline Long& Long::operator<<=(ddigit n)
{
    *this = *this << n;
    return *this;
}

inline Long& Long::operator>>=(ddigit n)
{
    *this = *this >> n;
    return *this;
}

inline Long& Long::operator+=(const Long& b)
{
    *this = *this + b;
    return *this;
}

inline Long& Long::operator-=(const Long& b)
{
    *this = *this - b;
    return *this;
}

inline Long& Long::operator*=(const Long& b)
{
    *this = *this * b;
    return *this;
}

inline Long& Long::operator/=(const Long& b)
{
    *this = *this / b;
    return *this;
}

inline Long& Long::operator%=(const Long& b)
{
    *this = *this % b;
    return *this;
}

inline bool Long::operator==(const Long& b) const
{
    return cmp(b) == 0;
}

inline bool Long::operator!=(const Long& b) const
{
    return cmp(b) != 0;
}

inline bool Long::operator>(const Long& b) const
{
    return cmp(b) > 0;
}

inline bool Long::operator<(const Long& b) const
{
    return cmp(b) < 0;
}

inline bool Long::operator>=(const Long& b) const
{
    return cmp(b) >= 0;
}

inline bool Long::operator<=(const Long& b) const
{
    return cmp(b) <= 0;
}


inline Long Long::operator+(ddigit b) const
{
    return *this + Long(b);
}

inline Long Long::operator-(ddigit b) const
{
    return *this - Long(b);
}

inline Long Long::operator*(ddigit b) const
{
    return *this * Long(b);
}

inline Long Long::operator/(ddigit b) const
{
    return *this / Long(b);
}

inline Long Long::operator%(ddigit b) const
{
    return *this % Long(b);
}

inline Long& Long::operator+=(ddigit b)
{
    *this = *this + b;
    return *this;
}

inline Long& Long::operator-=(ddigit b)
{
    *this = *this - b;
    return *this;
}

inline Long& Long::operator*=(ddigit b)
{
    *this = *this * b;
    return *this;
}

inline Long& Long::operator/=(ddigit b)
{
    *this = *this / b;
    return *this;
}

inline Long& Long::operator%=(ddigit b)
{
    *this = *this % b;
    return *this;
}

inline int Long::cmp(ddigit b) const
{
    return cmp(Long(b));
}

inline bool Long::operator==(ddigit b) const
{
    return cmp(b) == 0;
}

inline bool Long::operator!=(ddigit b) const
{
    return cmp(b) != 0;
}

inline bool Long::operator>(ddigit b) const
{
    return cmp(b) > 0;
}

inline bool Long::operator<(ddigit b) const
{
    return cmp(b) < 0;
}

inline bool Long::operator>=(ddigit b) const
{
    return cmp(b) >= 0;
}

inline bool Long::operator<=(ddigit b) const
{
    return cmp(b) <= 0;
}

}} // namespace ktn math
