/**
 * ulong.cpp - Unsigned BigInteger
 */
#include <cstdio>

#ifndef _MSC_VER
#include <cstdlib>
#include <cstring>
#endif

#include "ulong.h"

namespace mathktn {

static const ULong ZERO(0);
static const ULong ONE(1);

ULong::ULong() : l_(0), d_(NULL) {

}

ULong::~ULong() {
	if (d_) {
		delete [] d_;
	}
}

ULong::ULong(const ULong& l) {
	if (this == &l) { return; }
	l_ = l.l_;
	d_ = new int64_t[l_];
	for (int i = 0; i < l_; ++i) {
		d_[i] = l.d_[i];
	}
}

ULong::ULong(int64_t u) {
	if (u > 0xffff) {
		l_ = 2;
		d_ = new int64_t[2];
		if (!d_) { fprintf(stderr, "Failed new int64_t[2]"); return; }
		d_[0] = u & 0xffff;
		d_[1] = u >> 16;
	} else {
		l_ = 1;
		d_ = new int64_t[1];
		if (!d_) { fprintf(stderr, "Failed new int64_t[1]"); return; }
		d_[0] = u;
	}
}

ULong::ULong(const char *s, int base) {
	int index = 0;
	if (s[index] == '+') { ++index; }
	while (s[index] == '0') { ++index; }
	if (s[index] == '\0') {
		l_ = 1;
		d_ = new int64_t[1];
		if (!d_) { fprintf(stderr, "Failed new int64_t[1]"); return; }
		d_[0] = 0;
		return;
	}

	int length = strlen(s);
	int len = length + 1 - index;
	if (base == 8) {
		len *= 3;
	} else {
		len <<= 2;
	}
	len = (len >> 4) + 1;

	l_ = len;
	d_ = new int64_t[len];
	if (!d_) { fprintf(stderr, "Failed new int64_t[%d]", len); return; }
	for (int i = 0; i < len; ++i) {
		d_[i] = 0;
	}

	for (int bl = 1; ; ++index) {
		int64_t n = s[index] - '0';
		if (n > 9) { break; }
		for (int i = 0;;) {
			for (; i < bl; ++i) {
				n += d_[i] * base;
				d_[i] = n & 0xffff;
				n >>= 16;
			}
			if (n) {
				++bl;
			} else {
				break;
			}
		}
	}

	norm();
}

void ULong::alloc(int length, bool zero) {
	if (length < 1) {
		fprintf(stderr, "invalid value: ULong::alloc(%d, ...)", length);
		return;
	}
	if (l_ != length) {
		l_ = length;
		if (d_) { delete [] d_; }
		d_ = new int64_t[l_];
	}
	if (!zero) { return; }
	for (int i = 0; i < length; ++i) {
		d_[i] = 0;
	}
}

bool ULong::operator!() const {
	return l_ < 2 && d_[0] == 0;
}

ULong& ULong::operator++() {
	*this = *this + ONE;
	return *this;
}

ULong ULong::operator--(int) {
	ULong tmp(*this);
	--*this;
	return tmp;
}

ULong& ULong::operator--() {
	*this = *this + ONE;
	return *this;
}

ULong ULong::operator++(int) {
	ULong tmp(*this);
	++*this;
	return tmp;
}

ULong& ULong::operator=(const ULong& b) {
	if (this == &b) { return *this; }
	if (l_ < b.l_) {
		if (d_) { delete [] d_; }
		d_ = new int64_t[b.l_];
		if (!d_) { fprintf(stderr, "Failed new int64_t[%d]", b.l_); return *this; }
	}
	l_ = b.l_;
	for (int i = 0; i < l_; ++i) {
		d_[i] = b.d_[i];
	}

	return *this;
}

void ULong::debug() {
	for (int i = 0; i < l_; ++i) {
		printf("%lld ", d_[i]);
	}
	puts("");
}

inline void ULong::norm() {
	while (l_ > 1 && d_[l_ - 1] == 0) { --l_; }
}

inline void reverseChar(char* s) {
	if (*s == '\0') { return; }
	char *s0 = s;
	while (*s != '\0') { ++s; }
	--s;
	for (char t; s0 < s; --s, ++s0) {
		t = *s;
		*s = *s0;
		*s0 = t;
	}
}

std::string ULong::str(int base) {
	int length;
	if (l_ < 2) {
		length = 20;
	} else if (base == 8) {
		length = (l_ << 4) + 2;
	} else {
		length = (l_ * 241 / 50) + 2;
	}

	char* c = new char[length];
	if (!c) {
		fprintf(stderr, "Failed new char[%d]", length);
		std::string s;
		return s;
	}

	cstr(c, base);
	std::string s(c);
	delete [] c;

	return s;
}

void ULong::cstr(char *s, int base) {
	int i = l_;
	if (i < 2) {
#ifdef _MSC_VER
		_itoa_s(static_cast<int>(d_[0]), s, 19, base);
#else
		itoa(static_cast<int>(d_[0]), s, base);
#endif
		return;
	}

	int j, hbase;
	if (base == 8) {
		j = (i << 4) + 2;
		hbase = 0x1000;
	} else {
		j = (i * 241 / 50) + 2;
		hbase = 10000;
	}

	const char digits[] = "0123456789";
	ULong t(*this);
	int index = 0;
	
	while (i && j) {
		int k = i;
		int64_t n = 0;
		while (k--) {
			n = (n << 16) + t.d_[k];
			t.d_[k] = n / hbase;
			n %= hbase;
		}
		if (t.d_[i - 1] == 0) { --i; }
		k = 4;
		while (k--) {
			s[index] = digits[n % base];
			++index;
			--j;
			n /= base;
			if (i == 0 && n == 0) { break; }
		}
	}
	s[index] = '\0';

	reverseChar(s);
}

/**
 * output
 * @param base 2, 10, 16
 */
void ULong::out(int base, bool br) {
	if (base == 2) {
		int i = l_ - 1, j = 16;
		bool f = false;
		int64_t t;
		while (j--) {
			t = (d_[i] >> j) & 1;
			if (f) {
				//printf("%d", t);
				printf("%lld", t);
			} else if (t) {
				printf("%lld", t);
				f = true;
			}
		}
		if (i) {
			while (i--) {
				j = 16;
				while (j--) {
					printf("%lld", (d_[i] >> j) & 1);
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
				printf("%04x", d_[i]);
			}
		}
		if (br) { puts(""); }

		return;
	}
	char *c = (l_ > 1) ? new char[(l_ * 241 / 50) + 3] : new char[20];
	if (!c) { fprintf(stderr, "Failed new char[]"); return; }
	cstr(c, 10);
	printf(c);
	if (br) { puts(""); }
	delete [] c;
}

std::ostream& operator<<(std::ostream& os, ULong l) {
	return os << l.str(10);
}

int ULong::cmp(const ULong& b) const {
	if (this == &b) { return 0; }
	int al = l_;
	if (al < b.l_) { return -1; }
	if (al > b.l_) { return 1; }
	do { --al; } while (al && d_[al] == b.d_[al]);
	if (al == 0 && d_[0] == b.d_[0] ) { return 0; }
	return d_[al] > b.d_[al] ? 1 : -1;
}

ULong ULong::operator+(const ULong& b) const {
	if (l_ < b.l_) { return b + *this; }
	ULong z;
	z.alloc(l_ + 1, false);
	int i = 0;
	int64_t n = 0;
	for (; i < b.l_; ++i) {
		n += d_[i] + b.d_[i];
		z.d_[i] = n & 0xffff;
		n >>= 16;
	}
	for (; n && i < l_; ++i) {
		n += d_[i];
		z.d_[i] = n & 0xffff;
		n >>= 16;
	}
	for (; i < l_; ++i) {
		z.d_[i] = d_[i];
	}
	z.d_[i] = n & 0xffff;
	z.norm();
	return z;
}

ULong ULong::operator-(const ULong& b) const {
	ULong z;
	z.alloc(l_, false);
	int i = 0;
	int64_t c = 0;
	for (; i < b.l_; ++i) {
		if (d_[i] < b.d_[i] + c) {
			z.d_[i] = 0x10000 + d_[i] - b.d_[i] - c;
			c = 1;
		} else {
			z.d_[i] = d_[i] - b.d_[i] - c;
			c = 0;
		}
	}
	for (; i < l_; ++i) {
		if (d_[i] < c) {
			z.d_[i] = 0x10000 + d_[i] - c;
			c = 1;
		} else {
			z.d_[i] = d_[i] - c;
			c = 0;
		}
	}
	z.norm();
	return z;
}

ULong ULong::operator<<(int64_t n) const {
	int d = static_cast<int>(n >> 4);
	ULong c;
	c.alloc(l_ + d + 1, false);
	for (int i = 0; i < d; ++i) { c.d_[i] = 0; }
	int64_t b = n & 0xf, carry = 0;
	int i = 0;
	for (int64_t t = 0; i < l_; ++i) {
		t = (d_[i] << b) + carry;
		c.d_[i + d] = t & 0xffff;
		carry = t >> 16;
	}
	c.d_[i + d] = carry;
	c.norm();
	return c;
}

ULong ULong::operator>>(int64_t n) const {
	int d = static_cast<int>(n >> 4);
	if (l_ <= d) { return ZERO; }
	ULong c;
	c.alloc(l_ - d, false);
	int64_t b = n & 0xf;
	int i = 0;
	for (int64_t mask = (1 << b) - 1; i < l_ - d - 1; ++i) {
		c.d_[i] = ((d_[i + d + 1] & mask) << (16 - b)) + (d_[i + d] >> b);
	}
	c.d_[i] = d_[i + d] >> b;
	c.norm();
	return c;
}

ULong ULong::square() const {
	ULong s;
	s.alloc(l_ << 1, true);

	int64_t u, v ,uv, c;
	for (int i = 0; i < l_; ++i) {
		uv = s.d_[i << 1] + d_[i] * d_[i];
		u = uv >> 16;
		v = uv & 0xffff;
		s.d_[i << 1] = v;
		c = u;
		for (int j = i + 1; j < l_; ++j) {
			uv = d_[j] * d_[i];
			u = (uv >> 16) << 1;
			v = (uv & 0xffff) << 1;
			v += s.d_[i + j] + c;
			u += v >> 16;
			v &= 0xffff;
			s.d_[i + j] = v;
			c = u;
		}
		s.d_[i + l_] = u;
	}

	s.norm();
	return s;
}

/**
 * square root
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

ULong ULong::pow(int64_t n) const {
	ULong p(1), a(*this);
	for (; n > 0; n >>= 1, a = a.square()) {
		if (n & 1) { p *= a; }
	}
	return p;
}

inline int64_t ULong::bitLength() const {
	int64_t l = l_ << 4;
	int j = 16;
	while (j-- && ((d_[l_ - 1] >> j) & 1) == 0) {
		--l;
	}

	return l;
}

/**
 * multiple with karatsuba method
 */
ULong ULong::karatsuba(const ULong& u) const {
	int64_t N = bitLength();
	int64_t l = u.bitLength();
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
	ULong z;
	z.alloc(l_ + b.l_, true);

	int64_t n, e;
	for (int i = 0, j; i < l_; ++i) {
		if (d_[i] == 0) { continue; }
		n = 0;
		for (j = 0; j < b.l_; ++j) {
			e = n + d_[i] * b.d_[j];
			n = z.d_[i + j] + e;
			if (e) { z.d_[i + j] = n & 0xffff; }
			n >>= 16;
		}
		if (n) { z.d_[i + j] = n & 0xffff; }
	}

	z.norm();
	return z;
}

ULong ULong::random(int n) {
	ULong r;
	r.alloc(n, false);
	for (int i = 0; i < n; ++i) {
		r.d_[i] = rand() & 0xffff;
	}
	r.norm();
	return r;
}

/**
 * greatest common divisor
 */
ULong ULong::gcd(const ULong& b) const {
	ULong x(*this), y(b), z;
	for (;;) {
		z = x % y;
		if (!z) { break; }
		x = y;
		y = z;
	}

	return y;
}

/**
 * greatest common divisor with binary method
 */
ULong ULong::gcdBin(const ULong& b) const {
	if (cmp(b) > -1) { return b.gcdBin(*this); }
	ULong x(*this), y(b), g(1);
	while ((x.d_[0] & 1) == 0 && (y.d_[0] & 1) == 0) {
		x >>= 1;
		y >>= 1;
		g <<= 1;
	}

	while (!!x) {
		while ((x.d_[0] & 1) == 0) {
			x >>= 1;
		}
		while ((y.d_[0] & 1) == 0) {
			y >>= 1;
		}

		if (x.cmp(y) > -1) {
			x = (x - y) >> 1;
		} else {
			y = (y - x) >> 1;
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
	if (!b) {
		fprintf(stderr, "ZeroDivision: ULong::divmod");
		return ULong();
	}

	bool albl = l_ == b.l_;
	if (l_ < b.l_ || (albl && d_[l_ - 1] < b.d_[b.l_ - 1])) {
		if (mod) { return ULong(*this); }
		return ZERO;
	}

	if (b.l_ == 1) {
		int64_t dd = b.d_[0], t = 0;
		ULong z(*this);
		int i = l_;
		while (i--) {
			t = (t << 16) + z.d_[i];
			z.d_[i] = (t / dd) & 0xffff;
			t %= dd;
		}
		if (mod) {
			return ULong(t);
		}
		z.norm();
		return z;
	}

	ULong bb(b);
	ULong z;
	z.alloc(albl ? l_ + 2 : l_ + 1, true);
	int64_t dd = 0x10000 / (b.d_[b.l_ - 1] + 1) & 0xffff;
	
	if (dd == 1) {
		int j = l_;
		while (j--) { z.d_[j] = d_[j]; }
	} else {
		int64_t num = 0;
		int j;
		for (j = 0; j < b.l_; ++j) {
			num += b.d_[j] * dd;
			bb.d_[j] = num & 0xffff;
			num >>= 16;
		}

		num = 0;
		for (j = 0; j < l_; ++j) {
			num += d_[j] * dd;
			z.d_[j] = num & 0xffff;
			num >>= 16;
		}

		z.d_[j] = num & 0xffff;
	}

	int j = albl ? l_ + 1 : l_;
	do {
		int64_t q;
		if (z.d_[j] == bb.d_[bb.l_- 1]) {
			q = 0xffff;
		} else {
			q = ((z.d_[j] << 16) + z.d_[j - 1]) / bb.d_[bb.l_ - 1] & 0xffff;
		}

		if (q) {
			int i = 0;
			int64_t num = 0;
			int64_t t = 0;
			do {
				t += bb.d_[i] * q;
				int64_t ee = (t & 0xffff) - num;
				num = z.d_[j - bb.l_ + i] - ee;
				if (ee) {
					z.d_[j - bb.l_ + i] = num & 0xffff;
				}
				num >>= 16;
				t >>= 16;
			} while (++i < bb.l_);

			num += z.d_[j - bb.l_ + i] - t;
			while (num) {
				i = 0;
				num = 0;
				--q;

				do {
					int64_t ee = num + bb.d_[i];
					num = z.d_[j - bb.l_ + i] + ee;
					if (ee) { z.d_[j - bb.l_ + i] = num & 0xffff; }
					num >>= 16;
				} while (++i < bb.l_);

				--num;
			}
		}

		z.d_[j] = q;
	} while (--j >= bb.l_);

	ULong div(z);
	if (mod) {
		if (dd) {
			int64_t t = 0;
			int i = bb.l_;
			while (i--) {
				t = (t << 16) + div.d_[i];
				div.d_[i] = (t / dd) & 0xffff;
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

ULong& ULong::operator<<=(int64_t n) {
	*this = *this << n;
	return *this;
}

ULong& ULong::operator>>=(int64_t n) {
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
} // namespace mathktn