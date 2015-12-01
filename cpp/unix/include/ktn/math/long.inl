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

inline Long::Long(const Long& l)
{
	if (this == &l) { return; }

	s_ = l.s_;
	u_ = l.u_;
}

inline Long::Long(ddigit a)
{
	if (a < 0) {
		s_ = false;
		u_ = ULong(-a);
	}
	else {
		s_ = true;
		u_ = ULong(a);
	}
}

inline Long::Long(const char *s, int base)
{
	if (*s == '-') {
		s_ = false;
		++s;
	}
	else {
		s_ = true;
	}
	u_ = ULong(s, base);
}

inline void Long::debug() const
{
	if (!s_) {
		putchar('-');
	}
	u_.debug();
}

inline std::ostream& operator<<(std::ostream& os, const Long& l)
{
	return os << l.str(os.hex ? 16 : 10);
}

inline std::istream& operator>>(std::istream& is, Long& l)
{
	std::string str;
	is >> str;
	l = Long(str.c_str(), is.hex ? 16 : 10);
	return is;
}

inline std::string Long::str(int base) const
{
	std::string s = u_.str(base);
	if (!s_) { s = "-" + s; }

	return s;
}

inline void Long::cstr(char *s, int base) const
{
	if (!s_) {
		*s++ = '-';
	}
	u_.cstr(s, base);
}

inline void Long::out(int base) const
{
	if (!s_) { putchar('-'); }
	u_.out(base);
}

}} // namespace ktn math
