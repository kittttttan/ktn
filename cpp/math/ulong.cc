/**
 * @file  math/ulong.cc
 * @brief ULong
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ulong.h"

namespace ktn { namespace math {

#ifdef _WIN32
#define OUTPUT_FORMAT		"%I64d"
#else
#define OUTPUT_FORMAT		"%lld"
#endif
#define OUTPUT_FORMAT_B		"%04x"

#ifdef USE_64BIT
#define SHIFT_BIT	30
#define SHIFT_DEC	9
#define BASE_DEC	(1000000000)
#define getStringLength(l) (l * 241 / 25 + 2)
#else
#define SHIFT_BIT	15
#define SHIFT_DEC	4
#define BASE_DEC	(10000)
#define getStringLength(l) (l * 241 / 50 + 2)
#endif
#define BASE		(1 << SHIFT_BIT)
#define MASK		(BASE - 1)

const ULong ULong::ZERO(0);	/**< constant zero */
const ULong ULong::ONE(1);	/**< constant one */

ULong::ULong(const ULong& l) {
	if (this == &l) { return; }
	l_ = l.l_;
	d_ = new digit[l_];
	if (!d_) { l_ = 0; return; }
	for (int i = 0; i < l_; ++i) {
		d_[i] = l.d_[i];
	}
}

ULong::ULong(ddigit u) : l_(2) {
	d_ = new digit[2];
	if (!d_) { fprintf(stderr, "Failed new ddigit[2]"); return; }
	d_[0] = u & MASK;
	d_[1] = (u >> SHIFT_BIT) & MASK;
	norm();
}

/**
 * @param s     source string
 * @param radix 
 */
ULong::ULong(const char *s, int radix) {
	int index = 0;
	if (s[index] == '+') { ++index; }
	while (s[index] == '0') { ++index; }
	if (s[index] == '\0') {
		l_ = 0;
		d_ = NULL;
		return;
	}

	int len = strlen(s) + 1 - index;
	if (radix == 8) {
		len *= 3;
	} else {
		len <<= 2;
	}

	len = (len >> 4) + 1;

	l_ = len;
	d_ = new digit[len];
	if (!d_) { fprintf(stderr, "Failed new ddigit[%d]", len); return; }
	for (int i = 0; i < len; ++i) {
		d_[i] = 0;
	}

	for (int bl = 1; ; ++index) {
		ddigit n = s[index] - '0';
		if (n > 9 || n < 0) { break; }
		for (int i = 0;;) {
			for (; i < bl; ++i) {
				n += d_[i] * radix;
				d_[i] = n & MASK;
				n >>= SHIFT_BIT;
			}
			if (n != 0) {
				++bl;
			} else {
				break;
			}
		}
	}

	norm();
}

/**
 * Allocation.
 * @param length 
 * @param zero   fill zero flag
 */
void ULong::alloc(int length, bool zero) {
	if (length < 1) {
		fprintf(stderr, "invalid value: ULong::alloc(%d, ...)", length);
		return;
	}
	if (l_ != length) {
		l_ = length;
		delete [] d_;
		d_ = new digit[l_];
	}
	if (!zero) { return; }
	for (int i = 0; i < length; ++i) {
		d_[i] = 0;
	}
}

bool ULong::operator!() const {
	return l_ < 1 || (l_ == 1 && d_[0] == 0);
}

ULong ULong::operator+() const {
	return *this;
}

ULong& ULong::operator++() {
	*this = *this + ONE;
	return *this;
}

ULong ULong::operator--(int) {
	const ULong tmp(*this);
	--*this;
	return tmp;
}

ULong& ULong::operator--() {
	*this = *this + ONE;
	return *this;
}

ULong ULong::operator++(int) {
	const ULong tmp(*this);
	++*this;
	return tmp;
}

ULong& ULong::operator=(const ULong& b) {
	if (this == &b) { return *this; }
	if (l_ < b.l_) {
		delete [] d_;
		d_ = new digit[b.l_];
		if (!d_) {
			l_ = 0;
			fprintf(stderr, "Failed new ddigit[%d]", b.l_);
			return *this;
		}
	}
	l_ = b.l_;
	for (int i = 0; i < l_; ++i) {
		d_[i] = b.d_[i];
	}

	return *this;
}

void ULong::debug() const {
	for (int i = 0; i < l_; ++i) {
		printf(OUTPUT_FORMAT " ", d_[i]);
	}
	puts("");
}

/**
 * Fits length.
 */
inline void ULong::norm() {
	while (l_ > 0 && d_[l_ - 1] == 0) { --l_; }
}

inline void reverseChar(char* s) {
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

std::string ULong::str(int radix) const {
	if (!(*this)) {
		std::string s("0");
		return s;
	}
	int length;
	if (l_ < 2) {
		length = 20;
	} else if (radix == 8) {
		length = (l_ << 4) + 2;
	} else {
		length = getStringLength(l_);
	}

	char* c = new char[length];
	if (!c) {
		fprintf(stderr, "Failed new char[%d]", length);
		std::string s;
		return s;
	}

	cstr(c, radix);
	std::string s(c);
	delete [] c;

	return s;
}

/**
 * @param[out] s
 * @param[in]  radix 
 */
void ULong::cstr(char *s, int radix) const {
	if (!(*this)) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}

	const char digits[] = "0123456789";
	int i = l_, j;
	if (i < 2) {
		digit d = d_[0];
		j = 0;
		while (d) {
			s[j++] = digits[d % radix];
			d /= radix;
		}
		s[j] = '\0';
		reverseChar(s);
		return;
	}

	int hradix;
	if (radix == 8) {
		j = (i << 4) + 2;
		hradix = 0x1000;
	} else {
		j = getStringLength(i);
		hradix = 10000;
	}

	ULong t(*this);
	int index = 0;
	
	while (i && j) {
		int k = i;
		ddigit n = 0;
		while (k--) {
			n = (n << SHIFT_BIT) | t.d_[k];
			t.d_[k] = (n / hradix) & MASK;
			n %= hradix;
		}
		if (t.d_[i - 1] == 0) { --i; }
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

/**
 * output.
 * @param base 2, 10, 16
 * @param br   line break
 */
void ULong::out(int base, bool br) const {
	if (!(*this)) {
		putchar('0');
		if (br) { puts(""); }
		return;
	}
	if (base == 2) {
		int i = l_ - 1, j = SHIFT_BIT;
		bool f = false;
		ddigit t;
		while (j--) {
			t = (d_[i] >> j) & 1;
			if (f) {
				printf(OUTPUT_FORMAT, t);
			} else if (t) {
				printf(OUTPUT_FORMAT, t);
				f = true;
			}
		}
		if (i) {
			while (i--) {
				j = SHIFT_BIT;
				while (j--) {
					printf(OUTPUT_FORMAT, (d_[i] >> j) & 1);
				}
			}
		}
		if (br) { puts(""); }

		return;
	}
	if (base == 16) {
		int i = l_ - 1;
		printf("%x", d_[i]);
		if (i) {
			while (i--) {
				printf(OUTPUT_FORMAT_B, d_[i]);
			}
		}
		if (br) { puts(""); }

		return;
	}
	char *c = (l_ > 1) ? new char[getStringLength(l_) + 1] : new char[20];
	if (!c) { fprintf(stderr, "Failed new char[]"); return; }
	cstr(c, 10);
	printf("%s", c);
	if (br) { puts(""); }
	delete [] c;
}

std::ostream& operator<<(std::ostream& os, const ULong& l) {
	return os << l.str(10);
}
/*
std::istream& operator<<(std::istream& is, ULong& l) {
	return is;
}
*/
int ULong::cmp(const ULong& b) const {
	if (this == &b) { return 0; }
	int al = l_;
	if (al < b.l_) { return -1; }
	if (al > b.l_) { return 1; }
	if (al == 0) { return 0; }
	do { --al; } while (al && d_[al] == b.d_[al]);
	return d_[al] > b.d_[al] ? 1 :
			d_[al] < b.d_[al] ? -1 : 0;
}

ULong ULong::operator+(const ULong& b) const {
	if (b == ZERO) { return *this; }
	if (l_ < b.l_) { return b + *this; }
	ULong z;
	z.alloc(l_ + 1, false);
	int i = 0;
	ddigit n = 0;
	for (; i < b.l_; ++i) {
		n += d_[i] + b.d_[i];
		z.d_[i] = n & MASK;
		n >>= SHIFT_BIT;
	}
	for (; n && i < l_; ++i) {
		n += d_[i];
		z.d_[i] = n & MASK;
		n >>= SHIFT_BIT;
	}
	for (; i < l_; ++i) {
		z.d_[i] = d_[i];
	}
	z.d_[i] = n & MASK;
	z.norm();
	return z;
}

ULong ULong::operator-(const ULong& b) const {
	if (b == ZERO) { return *this; }
	ULong z;
	z.alloc(l_, false);
	int i = 0;
	digit c = 0;
	for (; i < b.l_; ++i) {
		if (d_[i] < b.d_[i] + c) {
			z.d_[i] = BASE + d_[i] - b.d_[i] - c;
			c = 1;
		} else {
			z.d_[i] = d_[i] - b.d_[i] - c;
			c = 0;
		}
	}
	for (; i < l_; ++i) {
		if (d_[i] < c) {
			z.d_[i] = BASE + d_[i] - c;
			c = 1;
		} else {
			z.d_[i] = d_[i] - c;
			c = 0;
		}
	}
	z.norm();
	return z;
}

ULong ULong::operator<<(ddigit n) const {
	const int d = static_cast<int>(n / SHIFT_BIT);
	ULong c;
	c.alloc(l_ + d + 1, false);
	for (int i = 0; i < d; ++i) { c.d_[i] = 0; }
	const int b = static_cast<int>(n % SHIFT_BIT);
	digit carry = 0;
	int i = 0;
	for (; i < l_; ++i) {
		const ddigit t = (d_[i] << b) + carry;
		c.d_[i + d] = t & MASK;
		carry = static_cast<int>(t >> SHIFT_BIT);
	}
	c.d_[i + d] = carry;
	c.norm();
	return c;
}

ULong ULong::operator>>(ddigit n) const {
	const int d = static_cast<int>(n / SHIFT_BIT);
	if (l_ <= d) { return ZERO; }
	ULong c;
	c.alloc(l_ - d, false);
	const int b = static_cast<int>(n % SHIFT_BIT);
	int i = 0;
	for (const digit mask = (1 << b) - 1; i < l_ - d - 1; ++i) {
		c.d_[i] = ((d_[i + d + 1] & mask) << (SHIFT_BIT - b)) + (d_[i + d] >> b);
	}
	c.d_[i] = d_[i + d] >> b;
	c.norm();
	return c;
}

ULong ULong::square() const {
	ULong s;
	s.alloc(l_ << 1, true);

	ddigit u, v ,uv, c;
	for (int i = 0; i < l_; ++i) {
		uv = s.d_[i << 1] + d_[i] * d_[i];
		u = uv >> SHIFT_BIT;
		v = uv & MASK;
		s.d_[i << 1] = static_cast<int>(v);
		c = u;
		for (int j = i + 1; j < l_; ++j) {
			uv = d_[j] * d_[i];
			u = (uv >> SHIFT_BIT) << 1;
			v = (uv & MASK) << 1;
			v += s.d_[i + j] + c;
			u += v >> SHIFT_BIT;
			v &= MASK;
			s.d_[i + j] = static_cast<int>(v);
			c = u;
		}
		s.d_[i + l_] = static_cast<int>(u);
	}

	s.norm();
	return s;
}

/**
 * square root.
 */
ULong ULong::sqrt() const {
	ULong b(*this), c(1);
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

ULong ULong::pow(ddigit n) const {
	ULong p(1), a(*this);
	for (; n > 0; n >>= 1, a = a.square()) {
		if (n & 1) { p *= a; }
	}
	return p;
}

inline ddigit ULong::bitLength() const {
	ddigit l = l_ * SHIFT_BIT;
	int j = SHIFT_BIT;
	while (j-- && ((d_[l_ - 1] >> j) & 1) == 0) {
		--l;
	}

	return l;
}

/**
 * multiple with karatsuba method.
 */
ULong ULong::karatsuba(const ULong& u) const {
	if (u == ZERO) { return ZERO; }
	if (u == ONE) { return *this; }
	ddigit N = bitLength();
	const ddigit l = u.bitLength();
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

ULong ULong::operator*(const ULong& b) const {
	if (b == ZERO) { return ZERO; }
	if (b == ONE) { return *this; }
	ULong z;
	z.alloc(l_ + b.l_, true);

	ddigit n, e;
	for (int i = 0, j; i < l_; ++i) {
		if (d_[i] == 0) { continue; }
		n = 0;
		for (j = 0; j < b.l_; ++j) {
			e = n + d_[i] * b.d_[j];
			n = z.d_[i + j] + e;
			if (e) { z.d_[i + j] = n & MASK; }
			n >>= SHIFT_BIT;
		}
		if (n) { z.d_[i + j] = n & MASK; }
	}

	z.norm();
	return z;
}

ULong ULong::random(int n) {
	ULong r;
	r.alloc(n, false);
	for (int i = 0; i < n; ++i) {
		r.d_[i] = rand() & MASK;
	}
	r.norm();
	return r;
}

/**
 * greatest common divisor.
 */
ULong ULong::gcd(const ULong& b) const {
	ULong x(*this), y(b);
	for (;;) {
		const ULong z(x % y);
		if (!z) { break; }
		x = y;
		y = z;
	}

	return y;
}

/**
 * greatest common divisor with binary method.
 */
ULong ULong::gcdBin(const ULong& b) const {
	if (*this < b) { return b.gcdBin(*this); }
	ULong x(*this), y(b), g(1);
	while ((x.d_[0] & 1) == 0 && (y.d_[0] & 1) == 0) {
		x >>= 1;
		y >>= 1;
		g <<= 1;
	}

	while (x != 0) {
		while ((x.d_[0] & 1) == 0) {
			x >>= 1;
		}
		while ((y.d_[0] & 1) == 0) {
			y >>= 1;
		}

		if (x < y) {
			y = (y - x) >> 1;
		} else {
			x = (x - y) >> 1;
		}
	}

	return g * y;
}

/**
 * @param b
 * @param mod
 * @return modulus if mod is true, else division
 */
ULong ULong::divmod(const ULong& b, bool mod) const {
	if (b == ZERO) {
		fprintf(stderr, "ZeroDivision: ULong::divmod");
		return ZERO;
	}
	if (b == ONE) {
		if (mod) { return ZERO; }
		return *this;
	}

	const bool albl = l_ == b.l_;
	if (l_ < b.l_ || (albl && d_[l_ - 1] < b.d_[b.l_ - 1])) {
		if (mod) { return *this; }
		return ZERO;
	}

	if (b.l_ == 1) {
		const ddigit dd = b.d_[0];
		ddigit t = 0;
		ULong z(*this);
		int i = l_;
		while (i--) {
			t = (t << SHIFT_BIT) | z.d_[i];
			z.d_[i] = (t / dd) & MASK;
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
	z.alloc(albl ? l_ + 2 : l_ + 1, true);
	const ddigit dd = BASE / (b.d_[b.l_ - 1] + 1) & MASK;
	
	if (dd == 1) {
		int j = l_;
		while (j--) { z.d_[j] = d_[j]; }
	} else {
		ddigit num = 0;
		int j;
		for (j = 0; j < b.l_; ++j) {
			num += b.d_[j] * dd;
			bb.d_[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		num = 0;
		for (j = 0; j < l_; ++j) {
			num += d_[j] * dd;
			z.d_[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		z.d_[j] = num & MASK;
	}

	int j = albl ? l_ + 1 : l_;
	do {
		ddigit q;
		if (z.d_[j] == bb.d_[bb.l_- 1]) {
			q = MASK;
		} else {
			q = ((z.d_[j] << SHIFT_BIT) | z.d_[j - 1]) / bb.d_[bb.l_ - 1] & MASK;
		}

		if (q != 0) {
			int i = 0;
			ddigit num = 0;
			ddigit t = 0;
			do {
				t += bb.d_[i] * q;
				const ddigit ee = (t & MASK) - num;
				num = z.d_[j - bb.l_ + i] - ee;
				if (ee) {
					z.d_[j - bb.l_ + i] = num & MASK;
				}
				num >>= SHIFT_BIT;
				t >>= SHIFT_BIT;
			} while (++i < bb.l_);

			num += z.d_[j - bb.l_ + i] - t;
			while (num != 0) {
				i = 0;
				num = 0;
				--q;

				do {
					const ddigit ee = num + bb.d_[i];
					num = z.d_[j - bb.l_ + i] + ee;
					if (ee) { z.d_[j - bb.l_ + i] = num & MASK; }
					num >>= SHIFT_BIT;
				} while (++i < bb.l_);

				--num;
			}
		}

		z.d_[j] = static_cast<int>(q);
	} while (--j >= bb.l_);

	ULong div(z);
	if (mod) {
		if (dd != 0) {
			ddigit t = 0;
			int i = bb.l_;
			while (i--) {
				t = (t << SHIFT_BIT) | div.d_[i];
				div.d_[i] = (t / dd) & MASK;
				t %= dd;
			}
		}

		div.l_ = bb.l_;
		div.norm();
		return div;
	}

	j = (albl ? l_ + 2 : l_ + 1) - bb.l_;
	for (int i = 0; i < j; ++i) { div.d_[i] = div.d_[i + bb.l_]; }
	div.l_ = j;
	div.norm();
	return div;
}

ULong ULong::operator/(const ULong& b) const {
	return divmod(b, false);
}

ULong ULong::operator%(const ULong& b) const {
	return divmod(b, true);
}

ULong& ULong::operator+=(const ULong& b) {
	*this = *this + b;
	return *this;
}

ULong& ULong::operator-=(const ULong& b) {
	*this = *this - b;
	return *this;
}

ULong& ULong::operator*=(const ULong& b) {
	*this = *this * b;
	return *this;
}

ULong& ULong::operator/=(const ULong& b) {
	*this = *this / b;
	return *this;
}

ULong& ULong::operator%=(const ULong& b) {
	*this = *this % b;
	return *this;
}

ULong& ULong::operator<<=(ddigit n) {
	*this = *this << n;
	return *this;
}

ULong& ULong::operator>>=(ddigit n) {
	*this = *this >> n;
	return *this;
}

bool ULong::operator==(const ULong& b) const {
	return cmp(b) == 0;
}

bool ULong::operator!=(const ULong& b) const {
	return cmp(b) != 0;
}

bool ULong::operator>(const ULong& b) const {
	return cmp(b) > 0;
}

bool ULong::operator<(const ULong& b) const {
	return cmp(b) < 0;
}

bool ULong::operator>=(const ULong& b) const {
	return cmp(b) >= 0;
}

bool ULong::operator<=(const ULong& b) const {
	return cmp(b) <= 0;
}


ULong ULong::operator+(ddigit b) const {
	return *this + ULong(b);
}

ULong ULong::operator-(ddigit b) const {
	return *this - ULong(b);
}

ULong ULong::operator*(ddigit b) const {
	return *this * ULong(b);
}

ULong ULong::operator/(ddigit b) const {
	return *this / ULong(b);
}

ULong ULong::operator%(ddigit b) const {
	return *this % ULong(b);
}

ULong& ULong::operator+=(ddigit b) {
	*this = *this + b;
	return *this;
}

ULong& ULong::operator-=(ddigit b) {
	*this = *this - b;
	return *this;
}

ULong& ULong::operator*=(ddigit b) {
	*this = *this * b;
	return *this;
}

ULong& ULong::operator/=(ddigit b) {
	*this = *this / b;
	return *this;
}

ULong& ULong::operator%=(ddigit b) {
	*this = *this % b;
	return *this;
}

int ULong::cmp(ddigit b) const {
	return cmp(ULong(b));
}

bool ULong::operator==(ddigit b) const {
	return cmp(b) == 0;
}

bool ULong::operator!=(ddigit b) const {
	return cmp(b) != 0;
}

bool ULong::operator>(ddigit b) const {
	return cmp(b) > 0;
}

bool ULong::operator<(ddigit b) const {
	return cmp(b) < 0;
}

bool ULong::operator>=(ddigit b) const {
	return cmp(b) >= 0;
}

bool ULong::operator<=(ddigit b) const {
	return cmp(b) <= 0;
}
}} // namespace ktn math