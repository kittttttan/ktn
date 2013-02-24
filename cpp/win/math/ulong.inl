#pragma once
#ifndef KTN_MATH_ULONG_INL_
#define KTN_MATH_ULONG_INL_

/**
 * @file  math/ulong.inl
 * @brief inline functions for ULong
 */

namespace ktn { namespace math {

/**
 * @param[out] s
 */
inline void reverseChar(char* s)
{
    if (*s == '\0') { return; }
    char *s0 = s;
    while (*s != '\0') { ++s; }
    --s;
    while (*s == '0' && s > s0) { --s; };
    *(s + 1) = '\0';
    for (char t; s0 < s; --s, ++s0) {
        t = *s;
        *s = *s0;
        *s0 = t;
    }
}

inline ddigit ULong::bitLength() const
{
    ddigit l = l_ * SHIFT_BIT;
    int j = SHIFT_BIT;
    while (j-- && ((d_[l_ - 1] >> j) & 1) == 0) {
        --l;
    }

    return l;
}

/**
 * Fits length.
 */
inline void ULong::norm()
{
    while (l_ > 0 && d_[l_ - 1] == 0) { --l_; }
}

/**
 * @param[in] l
 * @return
 */
inline int getStringLength(int l)
{
#ifdef USE_64BIT
    return l * 241 / 25 + 2;
#else
    return l * 241 / 50 + 2;
#endif
}

inline bool ULong::operator!() const
{
    return l_ < 1 || (l_ == 1 && d_[0] == 0);
}

inline ULong ULong::operator+() const
{
    return *this;
}

inline ULong& ULong::operator++()
{
    *this = *this + ONE;
    return *this;
}

inline ULong ULong::operator--(int)
{
    const ULong tmp(*this);
    --*this;
    return tmp;
}

inline ULong& ULong::operator--()
{
    *this = *this + ONE;
    return *this;
}

inline ULong ULong::operator++(int)
{
    const ULong tmp(*this);
    ++*this;
    return tmp;
}

inline ULong ULong::operator/(const ULong& b) const
{
    return divmod(b, false);
}

inline ULong ULong::operator%(const ULong& b) const
{
    return divmod(b, true);
}

inline ULong& ULong::operator+=(const ULong& b)
{
    *this = *this + b;
    return *this;
}

inline ULong& ULong::operator-=(const ULong& b)
{
    *this = *this - b;
    return *this;
}

inline ULong& ULong::operator*=(const ULong& b)
{
    *this = *this * b;
    return *this;
}

inline ULong& ULong::operator/=(const ULong& b)
{
    *this = *this / b;
    return *this;
}

inline ULong& ULong::operator%=(const ULong& b)
{
    *this = *this % b;
    return *this;
}

inline ULong& ULong::operator<<=(ddigit n)
{
    *this = *this << n;
    return *this;
}

inline ULong& ULong::operator>>=(ddigit n)
{
    *this = *this >> n;
    return *this;
}


inline ULong ULong::operator+(ddigit b) const
{
    return *this + ULong(b);
}

inline ULong ULong::operator-(ddigit b) const
{
    return *this - ULong(b);
}

inline ULong ULong::operator*(ddigit b) const
{
    return *this * ULong(b);
}

inline ULong ULong::operator/(ddigit b) const
{
    return *this / ULong(b);
}

inline ULong ULong::operator%(ddigit b) const
{
    return *this % ULong(b);
}

inline ULong& ULong::operator+=(ddigit b)
{
    *this = *this + b;
    return *this;
}

inline ULong& ULong::operator-=(ddigit b)
{
    *this = *this - b;
    return *this;
}

inline ULong& ULong::operator*=(ddigit b)
{
    *this = *this * b;
    return *this;
}

inline ULong& ULong::operator/=(ddigit b)
{
    *this = *this / b;
    return *this;
}

inline ULong& ULong::operator%=(ddigit b)
{
    *this = *this % b;
    return *this;
}

inline bool ULong::operator==(const ULong& b) const
{
    return cmp(b) == 0;
}

inline bool ULong::operator!=(const ULong& b) const
{
    return cmp(b) != 0;
}

inline bool ULong::operator>(const ULong& b) const
{
    return cmp(b) > 0;
}

inline bool ULong::operator<(const ULong& b) const
{
    return cmp(b) < 0;
}

inline bool ULong::operator>=(const ULong& b) const
{
    return cmp(b) >= 0;
}

inline bool ULong::operator<=(const ULong& b) const
{
    return cmp(b) <= 0;
}


inline int ULong::cmp(ddigit b) const
{
    return cmp(ULong(b));
}

inline bool ULong::operator==(ddigit b) const
{
    return cmp(b) == 0;
}

inline bool ULong::operator!=(ddigit b) const
{
    return cmp(b) != 0;
}

inline bool ULong::operator>(ddigit b) const
{
    return cmp(b) > 0;
}

inline bool ULong::operator<(ddigit b) const
{
    return cmp(b) < 0;
}

inline bool ULong::operator>=(ddigit b) const
{
    return cmp(b) >= 0;
}

inline bool ULong::operator<=(ddigit b) const
{
    return cmp(b) <= 0;
}

}} // namespace ktn math

#endif // KTN_MATH_ULONG_INL_
