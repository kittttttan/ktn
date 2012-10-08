/**
 * @file  math/long.cc
 * @brief Long
 */
#include "long.h"
#include "../dbg.h"

#include <cstdio>
#include <sstream>

namespace ktn { namespace math {

const Long Long::ZERO(0);
const Long Long::ONE(1);

Long::Long(const Long& l)
{
    if (this == &l) { return; }
    s_ = l.s_;
    u_ = l.u_;
}

Long::Long(ddigit a)
{
    if (a < 0) {
        s_ = false;
        u_ = ULong(-a);
    } else {
        s_ = true;
        u_ = ULong(a);
    }
}

Long::Long(const char *s, int base)
{
    if (*s == '-') {
        s_ = false;
        ++s;
    } else {
        s_ = true;
    }
    u_ = ULong(s, base);
}

bool Long::operator!() const
{
    return !u_;
}

Long Long::operator+() const
{
    return *this;
}

Long Long::operator-() const
{
    Long n(*this);
    n.s_ = !n.s_;

    return n;
}

Long& Long::operator++()
{
    *this = *this + ONE;
    return *this;
}

Long Long::operator++(int)
{
    Long tmp(*this);
    ++*this;
    return tmp;
}

Long& Long::operator--()
{
    *this = *this - ONE;
    return *this;
}

Long Long::operator--(int)
{
    Long tmp(*this);
    --*this;
    return tmp;
}

Long& Long::operator=(const Long& b)
{
    if (this == &b) { return *this; }
    s_ = b.s_;
    u_ = b.u_;

    return *this;
}

void Long::debug() const
{
    if (!s_) {
        putchar('-');
    }
    u_.debug();
}

std::ostream& operator<<(std::ostream& os, const Long& l)
{
    return os << l.str(os.hex ? 16 : 10);
}

std::istream& operator>>(std::istream& is, Long& l)
{
    std::string str;
    is >> str;
    l = Long(str.c_str(), is.hex ? 16 : 10);
    return is;
}

std::string Long::str(int base) const
{
    std::string s = u_.str(base);
    if (!s_) { s = "-" + s; }

    return s;
}

void Long::cstr(char *s, int base) const
{
    if (!s_) {
        *s++ = '-';
    }
    u_.cstr(s, base);
}

void Long::out(int base) const
{
    if (!s_) { putchar('-'); }
    u_.out(base);
}

int Long::cmp(const Long& b) const
{
    if (this == &b) { return 0; }
    if (s_ != b.s_) { return s_ ? 1 : -1; }
    return s_ ? u_.cmp(b.u_) : -u_.cmp(b.u_);
}

Long Long::operator+(const Long& a) const
{
    if (s_ == a.s_) {
        return Long(u_ + a.u_, s_);
    }
    if (u_ < a.u_) {
        return Long(a.u_ - u_, a.s_);
    }
    return Long(u_ - a.u_, s_);
}

Long Long::operator-(const Long& a) const
{
    if (s_ != a.s_) {
        return Long(u_ + a.u_, s_);
    }
    if (u_ < a.u_) {
        return Long(a.u_ - u_, !a.s_);
    }
    return Long(u_ - a.u_, s_);
}

Long Long::operator*(const Long& a) const
{
    return Long(u_ * a.u_, s_ == a.s_);
}

Long Long::operator/(const Long& a) const
{
    return Long(u_ / a.u_, s_ == a.s_);
}

Long Long::operator%(const Long& a) const
{
    return Long(u_ % a.u_, s_);
}

Long Long::operator<<(ddigit n) const
{
    return Long(u_ << n, true);
}

Long Long::operator>>(ddigit n) const
{
    return Long(u_ >> n, true);
}

Long& Long::operator<<=(ddigit n)
{
    *this = *this << n;
    return *this;
}

Long& Long::operator>>=(ddigit n)
{
    *this = *this >> n;
    return *this;
}

Long& Long::operator+=(const Long& b)
{
    *this = *this + b;
    return *this;
}

Long& Long::operator-=(const Long& b)
{
    *this = *this - b;
    return *this;
}

Long& Long::operator*=(const Long& b)
{
    *this = *this * b;
    return *this;
}

Long& Long::operator/=(const Long& b)
{
    *this = *this / b;
    return *this;
}

Long& Long::operator%=(const Long& b)
{
    *this = *this % b;
    return *this;
}

bool Long::operator==(const Long& b) const
{
    return cmp(b) == 0;
}

bool Long::operator!=(const Long& b) const
{
    return cmp(b) != 0;
}

bool Long::operator>(const Long& b) const
{
    return cmp(b) > 0;
}

bool Long::operator<(const Long& b) const
{
    return cmp(b) < 0;
}

bool Long::operator>=(const Long& b) const
{
    return cmp(b) >= 0;
}

bool Long::operator<=(const Long& b) const
{
    return cmp(b) <= 0;
}


Long Long::operator+(ddigit b) const
{
    return *this + Long(b);
}

Long Long::operator-(ddigit b) const
{
    return *this - Long(b);
}

Long Long::operator*(ddigit b) const
{
    return *this * Long(b);
}

Long Long::operator/(ddigit b) const
{
    return *this / Long(b);
}

Long Long::operator%(ddigit b) const
{
    return *this % Long(b);
}

Long& Long::operator+=(ddigit b)
{
    *this = *this + b;
    return *this;
}

Long& Long::operator-=(ddigit b)
{
    *this = *this - b;
    return *this;
}

Long& Long::operator*=(ddigit b)
{
    *this = *this * b;
    return *this;
}

Long& Long::operator/=(ddigit b)
{
    *this = *this / b;
    return *this;
}

Long& Long::operator%=(ddigit b)
{
    *this = *this % b;
    return *this;
}

int Long::cmp(ddigit b) const
{
    return cmp(Long(b));
}

bool Long::operator==(ddigit b) const
{
    return cmp(b) == 0;
}

bool Long::operator!=(ddigit b) const
{
    return cmp(b) != 0;
}

bool Long::operator>(ddigit b) const
{
    return cmp(b) > 0;
}

bool Long::operator<(ddigit b) const
{
    return cmp(b) < 0;
}

bool Long::operator>=(ddigit b) const
{
    return cmp(b) >= 0;
}

bool Long::operator<=(ddigit b) const
{
    return cmp(b) <= 0;
}

}} // namespace ktn math