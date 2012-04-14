#include "long.h"
#include "../string.h"

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

#define longNorm(self) {							\
	int l = abs(self->l_);							\
	while (l > 0 && self->d_[l - 1] == 0) { --l; }	\
	if (l == 0) {									\
		self->l_ = 0;								\
	} else {										\
		self->l_ = self->l_ < 0 ? -l : l;			\
	}												\
}

#define LONG_MALLOC_ERROR_CHECK(plong) {					\
	if (!plong->d_) {										\
		printf("malloc error [%d]%s\n",__LINE__,__FILE__);	\
		plong->a_ = plong->l_ = 0;							\
		return;												\
	}														\
}

void longInit(Long* self) {
	self->a_ = 0;
	self->l_ = 0;
	self->d_ = NULL;
}

void longAlloc(Long* self, int length) {
	int absLength = abs(length);
	self->l_ = length;
	if (self->a_ > absLength) { return; }
	if (self->d_ != NULL) { free(self->d_); }
	self->d_ = (digit*)malloc(sizeof(digit) * absLength);
	LONG_MALLOC_ERROR_CHECK(self);
	self->a_ = absLength;
}

void longFillZero(Long* self, int length) {
	longAlloc(self, length);
	memset(self->d_, 0, sizeof(digit) * abs(length));
}

void longNum(Long* self, ddigit n) {
	int d = 0;
	ddigit t = abs(n);
	while (t) {
		++d;
		t >>= SHIFT_BIT;
	}
	self->l_ = n < 0 ? -d : d;
	if (self->d_ != NULL) { free(self->d_); }
	self->d_ = (digit*)malloc(sizeof(digit) * d);
	LONG_MALLOC_ERROR_CHECK(self);
	self->a_ = d;
	d = 0;
	t = abs(n);
	while (t) {
		self->d_[d++] = t & MASK;
		t >>= SHIFT_BIT;
	}
	longNorm(self);
}

void longFree(Long* self) {
	self->a_ = self->l_ = 0;
	if (self->d_ != NULL) {
		free(self->d_);
		self->d_ = NULL;
	}
}

void longClone(Long* dest, const Long* src) {
	int destLength;
	if (dest == src) { return; }
	dest->l_ = src->l_;
	destLength = abs(src->l_);
	if (dest->a_ < destLength) {
		if (dest->d_ != NULL) { free(dest->d_); }
		dest->d_ = (digit*)malloc(sizeof(digit) * destLength);
		LONG_MALLOC_ERROR_CHECK(dest);
		dest->a_ = destLength;
	}
	memcpy(dest->d_, src->d_, sizeof(digit) * destLength);
}

/**
 * @param[out] s
 */
void longStr(const Long* self, char *s) {
	const int radix = 10, hradix = BASE_DEC;
	int i, j, k, index;
	ddigit n;
	const char digits[] = "0123456789";
	Long t;

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
		reverseString(s);
		return;
	}

	j = getStringLength(i);

	longInit(&t);
	longClone(&t, self);
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

	reverseString(s);

	longFree(&t);
}

/**
 * output.
 */
void longWrite(const Long* self) {
	char *c;
	int length;
	if (longIsZero(self)) {
		putchar('0');
		return;
	}

	length = (self->l_ > 1) ? getStringLength(self->l_) + 1 : 20;
	c = (char*)malloc(sizeof(char) * length);
	if (!c) { printf("Failed malloc char*"); return; }
	longStr(self, c);
	printf("%s", c);
	free(c);
}

/**
 * output.
 */
void longWriteln(const Long* self) {
	longWrite(self);
	puts("");
}

void longAdd(Long* dest, const Long* lhs, const Long* rhs) {
	int i;
	ddigit n;
	if (longIsZero(lhs)) {
		longClone(dest, rhs);
		return;
	}
	if (longIsZero(rhs)) {
		longClone(dest, lhs);
		return;
	}
	if (lhs->l_ < rhs->l_) {
		longAdd(dest, rhs, lhs);
		return;
	}

	longAlloc(dest, lhs->l_ + 1);
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
	longNorm(dest);
}

void longSub(Long* dest, const Long* lhs, const Long* rhs) {
	int i;
	digit c;

	if (longIsZero(rhs)) {
		longClone(dest, lhs);
		return;
	}

	longAlloc(dest, lhs->l_);

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
	longNorm(dest);
}

void longMul(Long* dest, const Long* lhs, const Long* rhs) {
	int i, j;
	ddigit n, e;
	if (longIsZero(rhs)) {
		longInit(dest);
		return;
	}
	if (longIsOne(rhs)) {
		longClone(dest, lhs);
		return;
	}

	longFillZero(dest, lhs->l_ + rhs->l_);

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

	longNorm(dest);
}


/**
 * @param b
 * @param mod
 * @return modulus if mod is true, else division
 */
void longDivmod(Long* dest, const Long* lhs, const Long* rhs, bool mod) {
	const bool albl = lhs->l_ == rhs->l_;
	int i, j;
	Long bb, div;
	ddigit dd, ee, t, num, q;

	if (longIsZero(rhs)) {
		printf("Zero Division @%d\n", __LINE__);
		return;
	}
	if (longIsOne(rhs)) {
		if (mod) {
			longInit(dest);
			return;
		}
		longClone(dest, lhs);
		return;
	}

	if (lhs->l_ < rhs->l_ || (albl && lhs->d_[lhs->l_ - 1] < rhs->d_[rhs->l_ - 1])) {
		if (mod) {
			longClone(dest, lhs);
			return;
		}
		longInit(dest);
		return;
	}

	if (rhs->l_ == 1) {
		dd = rhs->d_[0];
		t = 0;
		longClone(dest, lhs);
		i = lhs->l_;
		while (i--) {
			t = (t << SHIFT_BIT) | dest->d_[i];
			dest->d_[i] = (t / dd) & MASK;
			t %= dd;
		}
		if (mod) {
			longNum(dest, t);
			return;
		}
		longNorm(dest);
		return;
	}

	longInit(&bb);
	longClone(&bb, rhs);
	longFillZero(dest, albl ? lhs->l_ + 2 : lhs->l_ + 1);
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

	longInit(&div);
	longClone(&div, dest);
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
		longNorm((&div));
		longClone(dest, &div);
		longFree(&div);
		longFree(&bb);
		return;
	}

	j = (albl ? lhs->l_ + 2 : lhs->l_ + 1) - bb.l_;
	for (i = 0; i < j; ++i) { div.d_[i] = div.d_[i + bb.l_]; }
	div.l_ = j;
	longNorm((&div));
	longClone(dest, &div);
	longFree(&div);
	longFree(&bb);
}

void longLeftShift(Long* dest, const Long* self, ddigit n) {
	int i;
	const int d = (int)n / SHIFT_BIT;
	const ddigit b = n % SHIFT_BIT;
	ddigit t, carry = 0;

	longAlloc(dest, self->l_ + d + 1);
	memset(dest->d_, 0, sizeof(digit) * d);

	for (i = 0; i < self->l_; ++i) {
		t = (self->d_[i] << b) + carry;
		dest->d_[i + d] = t & MASK;
		carry = t >> SHIFT_BIT;
	}
	dest->d_[i + d] = (digit)carry;

	longNorm(dest);
}

void longRightShift(Long* dest, const Long* self, ddigit n) {
	int i = 0;
	const int d = (int)n / SHIFT_BIT;
	const ddigit b = n % SHIFT_BIT;
	const ddigit mask = (1 << b) - 1;

	if (self->l_ <= d) {
		longInit(dest);
		return;
	}

	longAlloc(dest, self->l_ - d);

	for (; i < self->l_ - d - 1; ++i) {
		dest->d_[i] = ((self->d_[i + d + 1] & mask) << (SHIFT_BIT - b)) + (self->d_[i + d] >> b);
	}
	dest->d_[i] = self->d_[i + d] >> b;

	longNorm(dest);
}

void longSquare(Long* dest, const Long* self) {
	ddigit u, v ,uv, c;
	int i, j;
	longFillZero(dest, self->l_ << 1);

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

	longNorm(dest);
}

/**
 * square root.
 */
void longSqrt(Long* dest, const Long* self) {
	Long c, t;
	longInit(&c);
	longInit(&t);
	longNum(&c, 1);
	longClone(dest, self);
	while (longGt(dest, &c)) {
		longRightShift(&t, dest, 1);
		longClone(dest, &t);

		longLeftShift(&t, &c, 1);
		longClone(&c, &t);
	}

	do {
		longClone(dest, &c);

		longDiv(&c, self, dest);
		longAdd(&t, &c, dest);
		longRightShift(&c, &t, 1);
	} while (longGt(dest, &c));
}

void longPow(Long* dest, const Long* self, ddigit n) {
	Long a, t;
	longInit(&a);
	longInit(&t);
	longClone(&a, self);
	longNum(dest, 1);
	for (; n > 0; n >>= 1) {
		if (n & 1) {
			longMul(&t, dest, &a);
			longClone(dest, &t);
		}

		longSquare(&t, &a);
		longClone(&a, &t);
	}
	longFree(&a);
	longFree(&t);
}

int longCmp(const Long* lhs, const Long* rhs) {
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
bool longEq(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) == 0;
}

bool longNe(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) != 0;
}

bool longLt(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) < 0;
}

bool longGt(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) > 0;
}

bool longLe(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) <= 0;
}

bool longGe(const Long* lhs, const Long* rhs) {
	return longCmp(lhs, rhs) >= 0;
}

#ifdef __cplusplus
}
#endif
