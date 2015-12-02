#pragma once

#include "ktn/dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

/*!
 @file  math/ulong.inl
 @brief inline functions for ULong
 */
namespace ktn { namespace math {

/*!
 @param[out] s
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

inline uddigit ULong::bitLength() const
{
    uddigit l = l_ * SHIFT_BIT;
    uddigit j = SHIFT_BIT;
    while (j-- && ((d_[l_ - 1] >> j) & 1) == 0) {
        --l;
    }

    return l;
}

/*!
 Fits length.
 */
inline void ULong::norm()
{
    while (l_ > 0 && d_[l_ - 1] == 0) { --l_; }
}

/*!
 @param[in] l
 @return
 */
inline size_t getStringLength(size_t l)
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

inline ULong& ULong::operator<<=(uddigit n)
{
    *this = *this << n;
    return *this;
}

inline ULong& ULong::operator>>=(uddigit n)
{
    *this = *this >> n;
    return *this;
}


inline ULong ULong::operator+(uddigit b) const
{
    return *this + ULong(b);
}

inline ULong ULong::operator-(uddigit b) const
{
    return *this - ULong(b);
}

inline ULong ULong::operator*(uddigit b) const
{
    return *this * ULong(b);
}

inline ULong ULong::operator/(uddigit b) const
{
    return *this / ULong(b);
}

inline ULong ULong::operator%(uddigit b) const
{
    return *this % ULong(b);
}

inline ULong& ULong::operator+=(uddigit b)
{
    *this = *this + b;
    return *this;
}

inline ULong& ULong::operator-=(uddigit b)
{
    *this = *this - b;
    return *this;
}

inline ULong& ULong::operator*=(uddigit b)
{
    *this = *this * b;
    return *this;
}

inline ULong& ULong::operator/=(uddigit b)
{
    *this = *this / b;
    return *this;
}

inline ULong& ULong::operator%=(uddigit b)
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


inline int ULong::cmp(uddigit b) const
{
    return cmp(ULong(b));
}

inline bool ULong::operator==(uddigit b) const
{
    return cmp(b) == 0;
}

inline bool ULong::operator!=(uddigit b) const
{
    return cmp(b) != 0;
}

inline bool ULong::operator>(uddigit b) const
{
    return cmp(b) > 0;
}

inline bool ULong::operator<(uddigit b) const
{
    return cmp(b) < 0;
}

inline bool ULong::operator>=(uddigit b) const
{
    return cmp(b) >= 0;
}

inline bool ULong::operator<=(uddigit b) const
{
    return cmp(b) <= 0;
}


/*!
Copy
@param[in] l
*/
inline ULong::ULong(const ULong& l)
{
	if (this == &l) { return; }

	const size_t len = l.l_;
	c_ = l_ = len;
	if (!len) { d_ = nullptr; return; }
	d_ = new udigit[len];
	::memcpy(d_, l.d_, sizeof(digit) * len);
}

/*!
@param[in] u
*/
inline ULong::ULong(uddigit u) :
	c_(0),
	l_(0),
	d_(nullptr)
{
	if (u == 0) { return; }

	const udigit high = static_cast<udigit>(u >> SHIFT_BIT);
	if (high) {
		c_ = l_ = 2;
		d_ = new udigit[2];
		d_[0] = static_cast<udigit>(u);
		d_[1] = high;
	}
	else {
		c_ = l_ = 1;
		d_ = new udigit[1];
		d_[0] = static_cast<udigit>(u);
	}
}

/*!
@param[in] s
@param[in] radix
*/
inline ULong::ULong(const char *s, int radix) :
	c_(0),
	l_(0),
	d_(nullptr)
{
	size_t index = 0;
	if (s[index] == '+') { ++index; }
	while (s[index] == '0') { ++index; }
	if (s[index] == '\0') {
		return;
	}

	size_t len = strlen(s) + 1 - index;
	if (radix == 8) {
		len *= 3;
	}
	else {
		len <<= 2;
	}

	len = (len >> 4) + 1;

	c_ = l_ = len;
	d_ = new udigit[len];
	::memset(d_, 0, sizeof(udigit) * len);

	for (size_t bl = 1; ; ++index) {
		uddigit n = s[index] - '0';
		if (n > 9/* || n < 0*/) { break; }

		for (size_t i = 0;;) {
			for (; i < bl; ++i) {
				n += d_[i] * radix;
				d_[i] = n & MASK;
				n >>= SHIFT_BIT;
			}
			if (n != 0) {
				++bl;
			}
			else {
				break;
			}
		}
	}

	norm();
}

/*!
@param[in] d
@param[in] size
*/
inline ULong::ULong(const udigit *d, size_t size) :
	c_(0),
	l_(0),
	d_(nullptr)
{
	if (size < 1) {
		return;
	}

	size_t i = size - 1;
	while (i > 0 && d[i] == 0) { --i; }
	if (d[i] == 0) {
		return;
	}

	++i;
	c_ = l_ = i;
	d_ = new udigit[i];
	::memcpy(d_, d, sizeof(udigit) * i);
}

/*!
Allocation.
@param[in] length
@param[in] zero   fill zero flag
*/
inline void ULong::alloc(size_t length, bool zero)
{
	if (l_ != length) {
		c_ = l_ = length;
		delete[] d_;
		d_ = new udigit[l_];
	}
	if (!zero) { return; }
	::memset(d_, 0, sizeof(udigit) * length);
}

/*!
@param[in] b
@return
*/
inline ULong& ULong::operator=(const ULong& b)
{
	if (this == &b) { return *this; }

	const size_t len = b.l_;
	if (l_ < len) {
		delete[] d_;
		d_ = new udigit[len];
		c_ = len;
	}
	else {
		::memset(d_ + len, 0, sizeof(udigit) * (l_ - len));
	}
	l_ = len;
	::memcpy(d_, b.d_, sizeof(udigit) * len);

	return *this;
}

/*!
output for debug
*/
inline void ULong::debug() const
{
	for (size_t i = 0; i < l_; ++i) {
		printf(FORMAT_DIGIT, d_[i]);
		printf(" ");
	}
	puts("");
}

/*!
@param[in] radix
@return
*/
inline std::string ULong::str(int radix) const
{
	if (!(*this)) {
		std::string s("0");
		return s;
	}

	size_t length;
	if (l_ < 2) {
		length = 20;
	}
	else if (radix == 8) {
		length = (l_ << 4) + 2;
	}
	else {
		length = getStringLength(l_);
	}

	char* c = new char[length];
	cstr(c, radix);
	std::string s(c);
	delete[] c;

	return s;
}

/*!
@param[out] s
@param[in]  radix
*/
inline void ULong::cstr(char *s, int radix) const
{
	if (!(*this)) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}

	const char digits[] = "0123456789abcdef";
	size_t i = l_;
	size_t j;
	if (i < 2) {
		udigit d = d_[0];
		j = 0;
		while (d) {
			s[j] = digits[d % radix];
			++j;
			d /= static_cast<udigit>(radix);
		}
		s[j] = '\0';
		reverseChar(s);
		return;
	}

	int hradix;
	switch (radix) {
	case 16:
		j = (i << 3) + 2;
		hradix = 0x10000;
		break;
	case 8:
		j = (i << 4) + 2;
		hradix = 0x1000;
		break;
	case 2:
		j = (i << 4) + 2;
		hradix = 0x10;
		break;
	case 10: default:
		j = getStringLength(i);
		hradix = 10000;
		break;
	}

	ULong t(*this);
	udigit* td = t.d_;
	size_t index = 0;
	while (i && j) {
		size_t k = i;
		uddigit n = 0;
		while (k--) {
			n = (n << SHIFT_BIT) | td[k];
			td[k] = static_cast<udigit>(n / hradix);// & MASK;
			n %= hradix;
		}
		if (td[i - 1] == 0) { --i; }
		k = 4;
		while (k--) {
			s[index] = digits[n % radix];
			++index;
			--j;
			n /= radix;
			if (i == 0 && n == 0) { break; }
		}
	}
	s[index] = '\0';

	reverseChar(s);
}

/*!
output.
@param[in] base 2, 10, 16
@param[in] br   line break
*/
inline void ULong::out(int base, bool br) const
{
	if (!(*this)) {
		putchar('0');
		if (br) { puts(""); }
		return;
	}

	const size_t al = l_;
	udigit* ad = d_;
	if (base == 2) {
		_ASSERTE(al > 0);
		size_t i = al - 1;
		uddigit j = SHIFT_BIT;
		bool f = false;
		uddigit t;
		while (j--) {
			t = (ad[i] >> j) & 1u;
			if (f) {
				printf(FORMAT_DDIGIT, t);
			}
			else if (t) {
				printf(FORMAT_DDIGIT, t);
				f = true;
			}
		}
		if (i) {
			while (i--) {
				j = SHIFT_BIT;
				while (j--) {
					printf(FORMAT_DIGIT, (ad[i] >> j) & 1u);
				}
			}
		}
		if (br) { puts(""); }

		return;
	}

	if (base == 16) {
		_ASSERTE(al > 0);
		size_t i = al - 1;
		printf("%x", ad[i]);
		if (i) {
			while (i--) {
				printf(FORMAT_B, ad[i]);
			}
		}
		if (br) { puts(""); }

		return;
	}

	char *c = new char[(al > 1) ? getStringLength(al) + 1 : 20];
	cstr(c, 10);
	printf("%s", c);
	if (br) { puts(""); }
	delete[] c;
}

/*!
@param[in,out] os
@param[in]     l
*/
inline std::ostream& operator<<(std::ostream& os, const ULong& l)
{
	return os << l.str(/*os.hex ? 16 : */10);
}

/*!
@param[in,out] is
@param[out]    l
*/
inline std::istream& operator>>(std::istream& is, ULong& l)
{
	std::string str;
	is >> str;
	l = ULong(str.c_str(), /*is.hex ? 16 : */10);
	return is;
}

/*!
@param[in] b
@return greater 1, equal 0, less -1
*/
inline int ULong::cmp(const ULong& b) const
{
	if (this == &b) { return 0; }

	size_t al = l_;
	if (al < b.l_) { return -1; }
	if (al > b.l_) { return 1; }
	if (al == 0) { return 0; }

	udigit* ad = d_;
	udigit* bd = b.d_;
	do { --al; } while (al && ad[al] == bd[al]);
	return ad[al] > bd[al] ? 1 :
		ad[al] < bd[al] ? -1 : 0;
}

inline ULong ULong::operator+(const ULong& b) const
{
	if (!b) { return *this; }

	const size_t l = l_;
	const size_t bl = b.l_;
	if (l < bl) { return b + *this; }

	ULong z;
	z.alloc(l + 1, false);
	udigit* zd = z.d_;
	udigit* ad = d_;
	udigit* bd = b.d_;

	size_t i = 0;
	uddigit n = 0;
	for (; i < bl; ++i) {
		n += ad[i] + bd[i];
		zd[i] = n & MASK;
		n >>= SHIFT_BIT;
	}
	for (; n && i < l; ++i) {
		n += ad[i];
		zd[i] = n & MASK;
		n >>= SHIFT_BIT;
	}
	for (; i < l_; ++i) {
		zd[i] = ad[i];
	}
	zd[i] = n & MASK;
	z.norm();

	return z;
}

inline ULong ULong::operator-(const ULong& b) const
{
	if (!b) { return *this; }

	const size_t l = l_;
	const size_t bl = b.l_;

	ULong z;
	z.alloc(l, false);
	udigit* zd = z.d_;
	udigit* ad = d_;
	udigit* bd = b.d_;

	size_t i = 0;
	udigit c = 0;
	for (; i < bl; ++i) {
		if (ad[i] < bd[i] + c) {
			zd[i] = static_cast<udigit>(BASE + ad[i] - bd[i] - c);
			c = 1;
		}
		else {
			zd[i] = ad[i] - bd[i] - c;
			c = 0;
		}
	}
	for (; i < l; ++i) {
		if (ad[i] < c) {
			zd[i] = static_cast<udigit>(BASE + ad[i] - c);
			c = 1;
		}
		else {
			zd[i] = ad[i] - c;
			c = 0;
		}
	}
	z.norm();

	return z;
}

inline ULong ULong::operator<<(uddigit n) const
{
	const size_t l = l_;
	const size_t d = n / SHIFT_BIT;
	ULong c;
	c.alloc(l + d + 1, false);
	udigit* cd = c.d_;
	::memset(cd, 0, sizeof(udigit) * d);
	udigit* ad = d_;

	const udigit b = n % SHIFT_BIT;
	uddigit carry = 0;
	size_t i = 0;
	for (; i < l; ++i) {
		const uddigit t = (ad[i] << b) + carry;
		cd[i + d] = t & MASK;
		carry = t >> SHIFT_BIT;
	}
	cd[i + d] = static_cast<udigit>(carry);
	c.norm();

	return c;
}

inline ULong ULong::operator>>(uddigit n) const
{
	const size_t d = n / SHIFT_BIT;
	const size_t l = l_;
	if (l <= d) { return ZERO; }

	ULong c;
	c.alloc(l - d, false);
	udigit* cd = c.d_;
	udigit* ad = d_;

	const udigit b = n % SHIFT_BIT;
	size_t i = 0;
	for (const udigit mask = (1 << b) - 1; i < l - d - 1; ++i) {
		cd[i] = ((ad[i + d + 1] & mask) << (SHIFT_BIT - b)) + (ad[i + d] >> b);
	}
	cd[i] = ad[i + d] >> b;
	c.norm();

	return c;
}

/*!
@return square
*/
inline ULong ULong::square() const
{
	const size_t t = l_;
	ULong s;
	s.alloc(t << 1, true);
	udigit* sd = s.d_;
	udigit* ad = d_;

	uddigit u, v, uv, c;
	for (size_t i = 0; i < t; ++i) {
		uv = ad[i] * ad[i] + sd[i << 1];
		u = uv >> SHIFT_BIT;
		v = uv & MASK;
		sd[i << 1] = static_cast<udigit>(v);
		c = u;
		for (size_t j = i + 1; j < t; ++j) {
			uv = ad[j] * ad[i];
			u = (uv >> SHIFT_BIT) << 1;
			v = (uv & MASK) << 1;
			v += sd[i + j] + c;
			u += v >> SHIFT_BIT;
			v &= MASK;
			sd[i + j] = static_cast<udigit>(v);
			c = u;
		}
		sd[i + t] = static_cast<udigit>(u);
	}

	s.norm();
	return s;
}

/*!
@return square root
*/
inline ULong ULong::sqrt() const
{
	ULong b(*this);
	ULong c(1);

	while (b > c) {
		b >>= 1;
		c <<= 1;
	}
	do {
		b = c;
		c = (*this / c + c) >> 1;
	} while (b > c);

	return b;
}

/*!
@param[in] n
@return this ^ n
*/
inline ULong ULong::pow(uddigit n) const
{
	ULong p(1);
	ULong a(*this);

	for (; n > 0; n >>= 1, a = a.square()) {
		if (n & 1) { p *= a; }
	}

	return p;
}

/*!
multiple with karatsuba method.
@param[in] u
@return this * u
*/
inline ULong ULong::karatsuba(const ULong& u) const
{
	if (!u) { return ZERO; }
	if (u == ONE) { return *this; }

	uddigit N = bitLength();
	const uddigit l = u.bitLength();
	if (N < l) { N = l; }
	if (N < 2000) { return *this * u; }

	N = (N >> 1) + (N & 1);

	ULong b(*this >> N);
	ULong a(*this - (b << N));
	ULong d(u >> N);
	ULong c(u - (d << N));
	ULong ac(a.karatsuba(c));
	ULong bd(b.karatsuba(d));
	ULong abcd((a + b).karatsuba(c + d));

	return ac + ((abcd - ac - bd) << N) + (bd << (N << 1));
}

inline ULong ULong::operator*(const ULong& b) const
{
	if (!b) { return ZERO; }
	if (b == ONE) { return *this; }

	const size_t l = l_;
	const size_t bl = b.l_;

	ULong z;
	z.alloc(l + bl, true);
	udigit* zd = z.d_;
	udigit* ad = d_;
	udigit* bd = b.d_;

	uddigit n, e;
	for (size_t i = 0, j; i < l; ++i) {
		if (ad[i] == 0) { continue; }

		n = 0;
		for (j = 0; j < bl; ++j) {
			e = n + ad[i] * bd[j];
			n = zd[i + j] + e;
			if (e) { zd[i + j] = n & MASK; }
			n >>= SHIFT_BIT;
		}
		if (n) { zd[i + j] = n & MASK; }
	}

	z.norm();
	return z;
}

/*!
@param[in] n
@return
*/
inline ULong ULong::random(size_t n)
{
	ULong r;
	r.alloc(n, false);
	udigit* rd = r.d_;

	for (size_t i = 0; i < n; ++i) {
		rd[i] = rand() & MASK;
	}
	r.norm();

	return r;
}

/*!
@param[in] n
@return odd part of n!
*/
inline ULong ULong::fact_odd(uddigit n)
{
	ULong m = ONE;
	uddigit mi, mj;
	uddigit i, j;
	uddigit l;
	const uddigit limit = static_cast<uddigit>(1) << (SHIFT_BIT);
	// const uddigit limit = 1 << (SHIFT_BIT << 1);

	for (i = 0; ; ++i) {
		l = (n / (static_cast<uddigit>(1) << i));
		if (l < 3) { break; }

		mi = mj = 1;
		for (j = 3; j <= l; j += 2) {
			mi *= j;
			if (mi > limit) {
				m *= mj;
				mi = mj = j;
			}
			else {
				mj = mi;
			}
		}

		if (mj > 1) { m *= mj; }
	}

	return m;
}

/*!
@param[in] n
@return even part of n!
*/
inline ULong ULong::fact_even(uddigit n)
{
	uddigit s = 0;

	while (n) {
		n >>= 1;
		s += n;
	}

	return ONE << s;
}

/*!
@param[in] n
@return n!
*/
inline ULong ULong::factorial(uddigit n)
{
	//ULong u = ONE;
	//for (int i = 2; i <= n; ++i) {
	//    u *= i;
	//}

	//return u;

	if (n < 2) return ONE;
	return fact_odd(n) * fact_even(n);
}

/*!
greatest common divisor.
@param[in] b
@return
*/
inline ULong ULong::gcd(const ULong& b) const
{
	ULong x(*this);
	ULong y(b);

	for (;;) {
		const ULong z(x % y);
		if (!z) { break; }
		x = y;
		y = z;
	}

	return y;
}

/*!
greatest common divisor with binary method.
@param[in] b
@return
*/
inline ULong ULong::gcdBin(const ULong& b) const
{
	if (*this < b) { return b.gcdBin(*this); }

	ULong x(*this);
	ULong y(b);
	ULong g(1);

	while (x.isEven() && y.isEven()) {
		x >>= 1;
		y >>= 1;
		g <<= 1;
	}

	while (!!x) {
		while (x.isEven()) {
			x >>= 1;
		}
		while (y.isEven()) {
			y >>= 1;
		}

		if (x < y) {
			y = (y - x) >> 1;
		}
		else {
			x = (x - y) >> 1;
		}
	}

	return g * y;
}

/*!
@param[in] b
@param[in] mod
@return modulus if mod is true, else division
*/
inline ULong ULong::divmod(const ULong& b, bool mod) const
{
	if (!b) {
		fprintf(stderr, "ZeroDivision: ULong::divmod");
		return ZERO;
	}
	if (b == ONE) {
		if (mod) { return ZERO; }
		return *this;
	}

	const size_t al = l_;
	const size_t bl = b.l_;
	udigit* ad = d_;
	udigit* bd = b.d_;
	const bool albl = al == bl;
	if (al < bl || (albl && ad[al - 1] < bd[bl - 1])) {
		if (mod) { return *this; }
		return ZERO;
	}

	if (bl == 1) {
		ULong z(*this);
		udigit* zd = z.d_;
		const uddigit dd = bd[0];
		uddigit t = 0;
		size_t i = al;
		while (i--) {
			t = (t << SHIFT_BIT) | zd[i];
			zd[i] = (t / dd) & MASK;
			t %= dd;
		}
		if (mod) {
			return ULong(t);
		}
		z.norm();
		return z;
	}

	const ULong bb(b);
	ULong z;
	z.alloc(albl ? al + 2 : al + 1, true);
	udigit* zd = z.d_;
	const uddigit dd = (BASE / (bd[bl - 1] + 1)) & MASK;

	if (dd == 1) {
		size_t j = al;
		while (j--) { zd[j] = ad[j]; }
	}
	else {
		uddigit num = 0;
		size_t j;
		for (j = 0; j < bl; ++j) {
			num += bd[j] * dd;
			bb.d_[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		bd = bb.d_;
		num = 0;
		for (j = 0; j < al; ++j) {
			num += ad[j] * dd;
			zd[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		zd[j] = num & MASK;
	}

	size_t j = albl ? al + 1 : al;
	do {
		uddigit q;
		if (zd[j] == bd[bl - 1]) {
			q = MASK;
		}
		else {
			q = (((static_cast<uddigit>(zd[j]) << SHIFT_BIT) | zd[j - 1]) / bd[bl - 1]) & MASK;
		}

		if (q) {
			size_t i = 0;
			ddigit num = 0;
			ddigit t = 0;
			do {
				t += bd[i] * q;
				//_ASSERTE((t & MASK) >= num);
				const ddigit ee = static_cast<ddigit>(t & MASK) - num;
				//_ASSERTE(zd[j - bl + i] >= ee);
				num = zd[j - bl + i] - ee;
				if (ee) {
					zd[j - bl + i] = num & MASK;
				}
				num >>= SHIFT_BIT;
				t >>= SHIFT_BIT;
			} while (++i < bl);

			//_ASSERTE(zd[j - bl + i] >= t);
			num += zd[j - bl + i] - t;
			while (num) {
				i = 0;
				num = 0;
				--q;

				do {
					const ddigit ee = num + bd[i];
					num = zd[j - bl + i] + ee;
					if (ee) { zd[j - bl + i] = num & MASK; }
					num >>= SHIFT_BIT;
				} while (++i < bl);

				--num;
			}
		}

		zd[j] = static_cast<udigit>(q);
	} while (--j >= bl);

	ULong div(z);
	udigit* dvd = div.d_;
	if (mod) {
		if (dd) {
			uddigit t = 0;
			size_t i = bl;
			while (i--) {
				t = (t << SHIFT_BIT) | dvd[i];
				dvd[i] = (t / dd) & MASK;
				t %= dd;
			}
		}

		div.l_ = bl;
		div.norm();
		return div;
	}

	j = (albl ? al + 2 : al + 1) - bl;
	for (size_t i = 0; i < j; ++i) { dvd[i] = dvd[i + bl]; }
	div.l_ = j;
	div.norm();
	return div;
}

}} // namespace ktn math
