#include "ulong.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef USE_64BIT
#define SHIFT_BIT	30
#define SHIFT_DEC	9
#define BASE_DEC	((digit)1000000000)
#define getStringLength(l) (l * 241 / 25 + 2)
#else
#define SHIFT_BIT	15
#define SHIFT_DEC	4
#define BASE_DEC	((digit)10000)
#define getStringLength(l) (l * 241 / 50 + 2)
#endif
#define BASE		((digit)1 << SHIFT_BIT)
#define MASK		((digit)(BASE - 1))

#define ulongNorm(self) {	\
	while (self->l_ > 0 && self->d_[self->l_ - 1] == 0) { --self->l_; }	\
}

void ulongInit(ULong* self) {
	self->l_ = 0;
	self->d_ = NULL;
}

void ulongAlloc(ULong* self, int length) {
	if (self->l_ == length) { return; }
	self->l_ = length;
	if (self->d_ != NULL) { free(self->d_); }
	self->d_ = (digit*)malloc(sizeof(digit) * length);
}

void ulongFillZero(ULong* self, int length) {
	ulongAlloc(self, length);
	memset(self->d_, 0, sizeof(digit) * length);
}

void ulongNum(ULong* self, ddigit n) {
	int d = 0;
	ddigit t = n;
	while (t) {
		++d;
		t >>= SHIFT_BIT;
	}
	self->l_ = d;
	if (self->d_ != NULL) { free(self->d_); }
	self->d_ = (digit*)malloc(sizeof(digit) * d);
	if (!self->d_) { printf("malloc error.\n"); return; }
	d = 0;
	t = n;
	while (t) {
		self->d_[d++] = t & MASK;
		t >>= SHIFT_BIT;
	}
	ulongNorm(self);
}

void ulongFree(ULong* self) {
	if (self->d_ != NULL) {
		free(self->d_);
		self->d_ = NULL;
	}
}

void ulongClone(ULong* dest, const ULong* src) {
	if (dest == src) { return; }
	if (dest->l_ != src->l_) {
		dest->l_ = src->l_;
		if (dest->d_ != NULL) { free(dest->d_); }
		dest->d_ = (digit*)malloc(sizeof(digit) * dest->l_);
		if (!dest->d_) { printf("malloc error. @%d\n",__LINE__); return; }
	}
	memcpy(dest->d_, src->d_, sizeof(digit) * dest->l_);
}

void reverseChar(char* s) {
	char t;
	char *s0 = s;
	if (*s == '\0') { return; }
	while (*s != '\0') { ++s; }
	--s;
	while (*s == '0' && s > s0) { --s; };
	*(s + 1) = '\0';
	for (; s0 < s; --s, ++s0) {
		t = *s;
		*s = *s0;
		*s0 = t;
	}
}

/**
 * @param[out] s
 */
void ulongStr(const ULong* self, char *s) {
	const int radix = 10, hradix = BASE_DEC;
	int i, j, k, index;
	ddigit n;
	const char digits[] = "0123456789";
	ULong t;

	if (self->l_ < 1) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}

	i = self->l_;
	if (i < 2) {
		digit d = self->d_[0];
		j = 0;
		while (d) {
			s[j++] = digits[d % radix];
			d /= radix;
		}
		s[j] = '\0';
		reverseChar(s);
		return;
	}

	j = getStringLength(i);

	ulongInit(&t);
	ulongClone(&t, self);
	index = 0;
	
	while (i && j) {
		k = i;
		n = 0;
		while (k--) {
			n = (n << SHIFT_BIT) | t.d_[k];
			t.d_[k] = (n / hradix) & MASK;
			n %= hradix;
		}
		if (t.d_[i - 1] == 0) { --i; }
		k = SHIFT_DEC;
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

	ulongFree(&t);
}

/**
 * output.
 */
void ulongWrite(const ULong* self) {
	char *c;
	int length;
	if (ulongIsZero(self)) {
		putchar('0');
		return;
	}

	length = (self->l_ > 1) ? getStringLength(self->l_) + 1 : 20;
	c = (char*)malloc(sizeof(char) * length);
	if (!c) { printf("Failed malloc char*"); return; }
	ulongStr(self, c);
	printf("%s", c);
	free(c);
}

/**
 * output.
 */
void ulongWriteln(const ULong* self) {
	ulongWrite(self);
	puts("");
}

void ulongAdd(ULong* dest, const ULong* lhs, const ULong* rhs) {
	int i;
	ddigit n;
	if (ulongIsZero(lhs)) {
		ulongClone(dest, rhs);
		return;
	}
	if (ulongIsZero(rhs)) {
		ulongClone(dest, lhs);
		return;
	}
	if (lhs->l_ < rhs->l_) {
		ulongAdd(dest, rhs, lhs);
		return;
	}

	ulongAlloc(dest, lhs->l_ + 1);
	i = 0;
	n = 0;
	for (; i < rhs->l_; ++i) {
		n += lhs->d_[i] + rhs->d_[i];
		dest->d_[i] = n & MASK;
		n >>= SHIFT_BIT;
		assert((n & 1) == n);
	}
	for (; n && i < lhs->l_; ++i) {
		n += lhs->d_[i];
		dest->d_[i] = n & MASK;
		n >>= SHIFT_BIT;
		assert((n & 1) == n);
	}
	for (; i < lhs->l_; ++i) {
		dest->d_[i] = lhs->d_[i];
	}
	dest->d_[i] = n & MASK;
	ulongNorm(dest);
}

void ulongSub(ULong* dest, const ULong* lhs, const ULong* rhs) {
	int i;
	digit c;

	if (ulongIsZero(rhs)) {
		ulongClone(dest, lhs);
		return;
	}

	ulongAlloc(dest, lhs->l_);

	i = 0;
	c = 0;
	for (; i < rhs->l_; ++i) {
		if (lhs->d_[i] < rhs->d_[i] + c) {
			dest->d_[i] = (digit)BASE + lhs->d_[i] - rhs->d_[i] - c;
			c = 1;
		} else {
			dest->d_[i] = (digit)lhs->d_[i] - rhs->d_[i] - c;
			c = 0;
		}
	}
	for (; i < lhs->l_; ++i) {
		if (lhs->d_[i] < c) {
			dest->d_[i] = (digit)BASE + lhs->d_[i] - c;
			c = 1;
		} else {
			dest->d_[i] = (digit)lhs->d_[i] - c;
			c = 0;
		}
	}
	ulongNorm(dest);
}

void ulongMul(ULong* dest, const ULong* lhs, const ULong* rhs) {
	int i, j;
	ddigit n, e;
	if (ulongIsZero(rhs)) {
		ulongInit(dest);
		return;
	}
	if (ulongIsOne(rhs)) {
		ulongClone(dest, lhs);
		return;
	}

	ulongFillZero(dest, lhs->l_ + rhs->l_);

	for (i = 0; i < lhs->l_; ++i) {
		if (lhs->d_[i] == 0) { continue; }
		n = 0;
		for (j = 0; j < rhs->l_; ++j) {
			e = n + lhs->d_[i] * rhs->d_[j];
			n = dest->d_[i + j] + e;
			if (e) { dest->d_[i + j] = n & MASK; }
			n >>= SHIFT_BIT;
		}
		if (n) { dest->d_[i + j] = n & MASK; }
	}

	ulongNorm(dest);
}


/**
 * @param b
 * @param mod
 * @return modulus if mod is true, else division
 */
void ulongDivmod(ULong* dest, const ULong* lhs, const ULong* rhs, bool mod) {
	const bool albl = lhs->l_ == rhs->l_;
	int i, j;
	ULong bb, div;
	ddigit dd, ee, t, num, q;

	if (ulongIsZero(rhs)) {
		printf("Zero Division @%d\n", __LINE__);
		return;
	}
	if (ulongIsOne(rhs)) {
		if (mod) {
			ulongInit(dest);
			return;
		}
		ulongClone(dest, lhs);
		return;
	}

	if (lhs->l_ < rhs->l_ || (albl && lhs->d_[lhs->l_ - 1] < rhs->d_[rhs->l_ - 1])) {
		if (mod) {
			ulongClone(dest, lhs);
			return;
		}
		ulongInit(dest);
		return;
	}

	if (rhs->l_ == 1) {
		dd = rhs->d_[0];
		t = 0;
		ulongClone(dest, lhs);
		i = lhs->l_;
		while (i--) {
			t = (t << SHIFT_BIT) + dest->d_[i];
			dest->d_[i] = (t / dd) & MASK;
			t %= dd;
		}
		if (mod) {
			ulongNum(dest, t);
			return;
		}
		ulongNorm(dest);
		return;
	}

	ulongInit(&bb);
	ulongClone(&bb, rhs);
	ulongFillZero(dest, albl ? lhs->l_ + 2 : lhs->l_ + 1);
	dd = BASE / (rhs->d_[rhs->l_ - 1] + 1) & MASK;
	
	if (dd == 1) {
		j = lhs->l_;
		while (j--) { dest->d_[j] = lhs->d_[j]; }
	} else {
		num = 0;
		for (j = 0; j < rhs->l_; ++j) {
			num += rhs->d_[j] * dd;
			bb.d_[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		num = 0;
		for (j = 0; j < lhs->l_; ++j) {
			num += lhs->d_[j] * dd;
			dest->d_[j] = num & MASK;
			num >>= SHIFT_BIT;
		}

		dest->d_[j] = num & MASK;
	}

	j = albl ? lhs->l_ + 1 : lhs->l_;
	do {
		if (dest->d_[j] == bb.d_[bb.l_- 1]) {
			q = MASK;
		} else {
			q = ((dest->d_[j] << SHIFT_BIT) + dest->d_[j - 1]) / bb.d_[bb.l_ - 1] & MASK;
		}

		if (q != 0) {
			i = 0;
			num = 0;
			t = 0;
			do {
				t += bb.d_[i] * q;
				ee = (t & MASK) - num;
				num = dest->d_[j - bb.l_ + i] - ee;
				if (ee) {
					dest->d_[j - bb.l_ + i] = num & MASK;
				}
				num >>= SHIFT_BIT;
				t >>= SHIFT_BIT;
			} while (++i < bb.l_);

			num += dest->d_[j - bb.l_ + i] - t;
			while (num != 0) {
				i = 0;
				num = 0;
				--q;

				do {
					ee = num + bb.d_[i];
					num = dest->d_[j - bb.l_ + i] + ee;
					if (ee) { dest->d_[j - bb.l_ + i] = num & MASK; }
					num >>= SHIFT_BIT;
				} while (++i < bb.l_);

				--num;
			}
		}

		dest->d_[j] = (digit)q;
	} while (--j >= bb.l_);

	ulongInit(&div);
	ulongClone(&div, dest);
	if (mod) {
		if (dd != 0) {
			t = 0;
			i = bb.l_;
			while (i--) {
				t = (t << SHIFT_BIT) + div.d_[i];
				div.d_[i] = (t / dd) & MASK;
				t %= dd;
			}
		}

		div.l_ = bb.l_;
		ulongNorm((&div));
		ulongClone(dest, &div);
		ulongFree(&div);
		ulongFree(&bb);
		return;
	}

	j = (albl ? lhs->l_ + 2 : lhs->l_ + 1) - bb.l_;
	for (i = 0; i < j; ++i) { div.d_[i] = div.d_[i + bb.l_]; }
	div.l_ = j;
	ulongNorm((&div));
	ulongClone(dest, &div);
	ulongFree(&div);
	ulongFree(&bb);
}

void ulongLeftShift(ULong* dest, const ULong* self, ddigit n) {
	int i;
	const int d = (int)n / SHIFT_BIT;
	const ddigit b = n % SHIFT_BIT;
	ddigit t, carry = 0;

	ulongAlloc(dest, self->l_ + d + 1);
	memset(dest->d_, 0, sizeof(digit) * d);

	for (i = 0; i < self->l_; ++i) {
		t = (self->d_[i] << b) + carry;
		dest->d_[i + d] = t & MASK;
		carry = t >> SHIFT_BIT;
	}
	dest->d_[i + d] = (digit)carry;

	ulongNorm(dest);
}

void ulongRightShift(ULong* dest, const ULong* self, ddigit n) {
	int i = 0;
	const int d = (int)n / SHIFT_BIT;
	const ddigit b = n % SHIFT_BIT;
	const ddigit mask = (1 << b) - 1;

	if (self->l_ <= d) {
		ulongInit(dest);
		return;
	}

	ulongAlloc(dest, self->l_ - d);

	for (; i < self->l_ - d - 1; ++i) {
		dest->d_[i] = ((self->d_[i + d + 1] & mask) << (SHIFT_BIT - b)) + (self->d_[i + d] >> b);
	}
	dest->d_[i] = self->d_[i + d] >> b;

	ulongNorm(dest);
}

void ulongSquare(ULong* dest, const ULong* self) {
	ddigit u, v ,uv, c;
	int i, j;
	ulongFillZero(dest, self->l_ << 1);

	for (i = 0; i < self->l_; ++i) {
		uv = dest->d_[i << 1] + self->d_[i] * self->d_[i];
		u = uv >> SHIFT_BIT;
		v = uv & MASK;
		dest->d_[i << 1] = (digit)v;
		c = u;
		for (j = i + 1; j < self->l_; ++j) {
			uv = self->d_[j] * self->d_[i];
			u = (uv >> SHIFT_BIT) << 1;
			v = (uv & MASK) << 1;
			v += dest->d_[i + j] + c;
			u += v >> SHIFT_BIT;
			v &= MASK;
			dest->d_[i + j] = (digit)v;
			c = u;
		}
		dest->d_[i + self->l_] = (digit)u;
	}

	ulongNorm(dest);
}

/**
 * square root.
 */
void ulongSqrt(ULong* dest, const ULong* self) {
	ULong c, t;
	ulongInit(&c);
	ulongInit(&t);
	ulongNum(&c, 1);
	ulongClone(dest, self);
	while (ulongGt(dest, &c)) {
		ulongRightShift(&t, dest, 1);
		ulongClone(dest, &t);

		ulongLeftShift(&t, &c, 1);
		ulongClone(&c, &t);
	}

	do {
		ulongClone(dest, &c);

		ulongDiv(&c, self, dest);
		ulongAdd(&t, &c, dest);
		ulongRightShift(&c, &t, 1);
	} while (ulongGt(dest, &c));
}

void ulongPow(ULong* dest, const ULong* self, ddigit n) {
	ULong a, t;
	ulongInit(&a);
	ulongInit(&t);
	ulongClone(&a, self);
	ulongNum(dest, 1);
	for (; n > 0; n >>= 1) {
		if (n & 1) {
			ulongMul(&t, dest, &a);
			ulongClone(dest, &t);
		}

		ulongSquare(&t, &a);
		ulongClone(&a, &t);
	}
	ulongFree(&a);
	ulongFree(&t);
}

int ulongCmp(const ULong* lhs, const ULong* rhs) {
	int al;
	if (lhs == rhs) { return 0; }
	al = lhs->l_;
	if (al < rhs->l_) { return -1; }
	if (al > rhs->l_) { return 1; }
	if (al == 0) { return 0; }
	do { --al; } while (al && lhs->d_[al] == rhs->d_[al]);
	return lhs->d_[al] > rhs->d_[al] ? 1 :
			lhs->d_[al] < rhs->d_[al] ? -1 : 0;

}
bool ulongEq(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) == 0;
}

bool ulongNe(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) != 0;
}

bool ulongLt(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) < 0;
}

bool ulongGt(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) > 0;
}

bool ulongLe(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) <= 0;
}

bool ulongGe(const ULong* lhs, const ULong* rhs) {
	return ulongCmp(lhs, rhs) >= 0;
}

#ifdef __cplusplus
}
#endif
