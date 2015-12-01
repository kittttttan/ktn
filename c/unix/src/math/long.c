#include "ktn/math/long.h"
#include "ktn/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_64BIT
#define getStringLength(l) ((l) * 241 / 25 + 2)
#else
#define getStringLength(l) ((l) * 241 / 50 + 2)
#endif

enum {
#ifdef USE_64BIT
  SHIFT_BIT = 30,
  SHIFT_DEC = 9,
#else
  SHIFT_BIT = 15,
  SHIFT_DEC = 4,
#endif
  BASE = 1 << SHIFT_BIT,
  MASK = BASE - 1,
};

#define longNorm(self) {                            \
  int l = abs((self)->l_);                          \
  while (l > 0 && (self)->d_[l - 1] == 0) { --l; }  \
  if (l == 0) {                                     \
    (self)->l_ = 0;                                 \
  } else {                                          \
    (self)->l_ = (self)->l_ < 0 ? -l : l;           \
  }                                                 \
}

#define LONG_MALLOC_ERROR_CHECK(plong) {                        \
  if (!(plong)->d_) {                                           \
    fprintf(stderr,"malloc error [%d]%s\n",__LINE__,__FILE__);  \
    (plong)->a_ = (plong)->l_ = 0;                              \
    return;                                                     \
  }                                                             \
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

void longAbs(Long* dest, const Long* src) {
  longClone(dest, src);
  dest->l_ = abs(dest->l_);
}

void longNeg(Long* dest, const Long* src) {
  longClone(dest, src);
  dest->l_ = -dest->l_;
}

/**
 * @param[out] s
 */
void longStr(const Long* self, char *s) {
  const digit radix = 10;
  const int length = abs(self->l_);
  int i, j, k, index;
  ddigit n, hradix;
  const char digits[] = "0123456789abcdef";
  Long t;
  digit *td;

  if (length < 1) {
    s[0] = '0';
    s[1] = '\0';
    return;
  }

  i = length;
  if (i < 2) {
    digit d = self->d_[0];
    j = 0;
    while (d) {
      s[j++] = digits[d % radix];
      d /= radix;
    }
    if (longIsNeg(self)) { s[j] = '-'; ++j; }
    s[j] = '\0';
    reverseString(s);
    return;
  }

  j = getStringLength(i);
  hradix = 10000;

  longInit(&t);
  longClone(&t, self);
  td = t.d_;
  index = 0;
  
  while (i && j) {
    k = i;
    n = 0;
    while (k--) {
      n = (n << SHIFT_BIT) | td[k];
      td[k] = (n / hradix) & MASK;
      n %= hradix;
    }
    if (td[i - 1] == 0) { --i; }
    k = SHIFT_DEC;
    while (k--) {
      s[index] = digits[n % radix];
      ++index;
      --j;
      n /= radix;
      if (i == 0 && n == 0) { break; }
    }
  }
  if (longIsNeg(self)) { s[index] = '-'; ++index; }
  s[index] = '\0';

  reverseString(s);

  longFree(&t);
}

/**
 * output.
 */
void longWrite(const Long* self) {
  char *c;
  int length, abs_l;

  if (longIsZero(self)) {
    putchar('0');
    return;
  }

  abs_l = abs(self->l_);
  length = (abs_l > 1) ? getStringLength(abs_l) + 1 : 20;
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

/**
 * |lhs| + |rhs|
 */
void longAddAbs(Long* dest, const Long* lhs, const Long* rhs) {
  int i, ll, rl;
  ddigit n;
  digit *dd, *ld, *rd;

  if (abs(lhs->l_) < abs(rhs->l_)) {
    ll = abs(rhs->l_);
    rl = abs(lhs->l_);
    longAlloc(dest, longIsNeg(rhs) ? -(ll + 1) : ll + 1);
    ld = rhs->d_;
    rd = lhs->d_;
  } else {
    ll = abs(lhs->l_);
    rl = abs(rhs->l_);
    longAlloc(dest, longIsNeg(lhs) ? -(ll + 1) : ll + 1);
    ld = lhs->d_;
    rd = rhs->d_;
  }

  dd = dest->d_;
  ll = abs(ll);
  i = 0;
  n = 0;
  for (; i < rl; ++i) {
    n += ld[i] + rd[i];
    dd[i] = n & MASK;
    n >>= SHIFT_BIT;
    assert((n & 1) == n);
  }
  for (; n && i < ll; ++i) {
    n += ld[i];
    dd[i] = n & MASK;
    n >>= SHIFT_BIT;
    assert((n & 1) == n);
  }
  for (; i < ll; ++i) {
    dd[i] = ld[i];
  }
  dd[i] = n & MASK;
  longNorm(dest);
}

void longAdd(Long* dest, const Long* lhs, const Long* rhs) {
  const int sign = lhs->l_ ^ rhs->l_;

  // TODO
  if (sign < 0) {
    longSubAbs(dest, lhs, rhs);
  } else {
    longAddAbs(dest, lhs, rhs);
  }
}

/**
 * |lhs| - |rhs|
 */
void longSubAbs(Long* dest, const Long* lhs, const Long* rhs) {
  int i, ll, rl;
  digit c;
  digit *dd, *ld, *rd;
  const int cmp = longCmpAbs(lhs, rhs);

  if (cmp == 0) {
    longInit(dest);
    return;
  }

  if (cmp < 1) {
    ll = abs(rhs->l_);
    rl = abs(lhs->l_);
    longAlloc(dest, longIsNeg(rhs) ? -ll : ll);
    ld = rhs->d_;
    rd = lhs->d_;
  } else {
    ll = abs(lhs->l_);
    rl = abs(rhs->l_);
    longAlloc(dest, longIsNeg(lhs) ? -ll : ll);
    ld = lhs->d_;
    rd = rhs->d_;
  }

  dd = dest->d_;
  i = 0;
  c = 0;
  for (; i < rl; ++i) {
    if (ld[i] < rd[i] + c) {
      dd[i] = (digit)(BASE + ld[i] - rd[i] - c);
      c = 1;
    } else {
      dd[i] = (digit)(ld[i] - rd[i] - c);
      c = 0;
    }
  }
  for (; i < ll; ++i) {
    if (ld[i] < c) {
      dd[i] = (digit)(BASE + ld[i] - c);
      c = 1;
    } else {
      dd[i] = (digit)(ld[i] - c);
      c = 0;
    }
  }
  longNorm(dest);
}

void longSub(Long* dest, const Long* lhs, const Long* rhs) {
  const int sign = lhs->l_ ^ rhs->l_;

  if (sign < 0) {
    longAddAbs(dest, lhs, rhs);
  } else {
    longSubAbs(dest, lhs, rhs);
  }
}

void longMul(Long* dest, const Long* lhs, const Long* rhs) {
  int i, j;
  ddigit n, e;
  digit *dd, *ld, *rd;
  int sign = rhs->l_ ^ lhs->l_;
  int ll = abs(lhs->l_), rl = abs(rhs->l_);

  longFillZero(dest, sign < 0 ? -(ll + rl) : ll + rl);

  dd = dest->d_;
  ld = lhs->d_;
  rd = rhs->d_;
  for (i = 0; i < ll; ++i) {
    if (ld[i] == 0) { continue; }
    n = 0;
    for (j = 0; j < rl; ++j) {
      e = n + ld[i] * rd[j];
      n = dd[i + j] + e;
      if (e) { dd[i + j] = n & MASK; }
      n >>= SHIFT_BIT;
    }
    if (n) { dd[i + j] = n & MASK; }
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
  int i, j, ll, rl, bbl, sign;
  Long bb, div;
  ddigit dd, ee, t, num, q;

  if (longIsZero(rhs)) {
    fprintf(stderr, "Zero Division @%d\n", __LINE__);
    return;
  }
  if (longIsOne(rhs)) {
    if (mod) {
      longInit(&bb);
      longClone(dest, &bb);
      longFree(&bb);
      return;
    }
    longClone(dest, lhs);
    return;
  }

  ll = abs(lhs->l_);
  rl = abs(rhs->l_);
  if (ll < rl || (albl && lhs->d_[ll - 1] < rhs->d_[rl - 1])) {
    if (mod) {
      longClone(dest, lhs);
      return;
    }
    longInit(&bb);
    longClone(dest, &bb);
    longFree(&bb);
    return;
  }

  if (rl == 1) {
    dd = rhs->d_[0];
    t = 0;
    longClone(dest, lhs);
    i = ll;
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
    if (longIsNeg(rhs)) { dest->l_ = -dest->l_; }
    return;
  }

  longInit(&bb);
  longClone(&bb, rhs);
  bbl = abs(bb.l_);
  longFillZero(dest, albl ? ll + 2 : ll + 1);
  dd = BASE / (rhs->d_[rl - 1] + 1) & MASK;
  
  if (dd == 1) {
    j = ll;
    while (j--) { dest->d_[j] = lhs->d_[j]; }
  } else {
    num = 0;
    for (j = 0; j < rl; ++j) {
      num += rhs->d_[j] * dd;
      bb.d_[j] = num & MASK;
      num >>= SHIFT_BIT;
    }

    num = 0;
    for (j = 0; j < ll; ++j) {
      num += lhs->d_[j] * dd;
      dest->d_[j] = num & MASK;
      num >>= SHIFT_BIT;
    }

    dest->d_[j] = num & MASK;
  }

  j = albl ? ll + 1 : ll;
  do {
    if (dest->d_[j] == bb.d_[bbl - 1]) {
      q = MASK;
    } else {
      q = ((dest->d_[j] << SHIFT_BIT) + dest->d_[j - 1]) / bb.d_[bbl - 1] & MASK;
    }

    if (q != 0) {
      i = 0;
      num = 0;
      t = 0;
      do {
        t += bb.d_[i] * q;
        ee = (t & MASK) - num;
        num = dest->d_[j - bbl + i] - ee;
        if (ee) {
          dest->d_[j - bbl + i] = num & MASK;
        }
        num >>= SHIFT_BIT;
        t >>= SHIFT_BIT;
      } while (++i < bbl);

      num += dest->d_[j - bbl + i] - t;
      while (num != 0) {
        i = 0;
        num = 0;
        --q;

        do {
          ee = num + bb.d_[i];
          num = dest->d_[j - bbl + i] + ee;
          if (ee) { dest->d_[j - bbl + i] = num & MASK; }
          num >>= SHIFT_BIT;
        } while (++i < bbl);

        --num;
      }
    }

    dest->d_[j] = (digit)q;
  } while (--j >= bbl);

  longInit(&div);
  longClone(&div, dest);
  if (mod) {
    if (dd != 0) {
      t = 0;
      i = bbl;
      while (i--) {
        t = (t << SHIFT_BIT) + div.d_[i];
        div.d_[i] = (t / dd) & MASK;
        t %= dd;
      }
    }

    div.l_ = longIsNeg(lhs) ? -bb.l_ : bb.l_;
    longNorm((&div));
    longClone(dest, &div);
    longFree(&div);
    longFree(&bb);
    return;
  }

  sign = lhs->l_ ^ rhs->l_;

  j = (albl ? ll + 2 : ll + 1) - bbl;
  for (i = 0; i < j; ++i) { div.d_[i] = div.d_[i + bbl]; }
  div.l_ = sign < 0 ? -j : j;
  longNorm((&div));
  longClone(dest, &div);
  longFree(&div);
  longFree(&bb);
}

void longLeftShift(Long* dest, const Long* self, ddigit n) {
  int i;
  ddigit t, carry = 0;
  digit *dd, *sd;
  const int length = abs(self->l_);
  const int d = (int)n / SHIFT_BIT;
  const ddigit b = n % SHIFT_BIT;

  longAlloc(dest, longIsNeg(self) ? -(length + d + 1) : length + d + 1);
  memset(dest->d_, 0, sizeof(digit) * d);

  dd = dest->d_;
  sd = self->d_;
  for (i = 0; i < length; ++i) {
    t = (sd[i] << b) + carry;
    dd[i + d] = t & MASK;
    carry = t >> SHIFT_BIT;
  }
  dd[i + d] = (digit)carry;

  longNorm(dest);
}

void longRightShift(Long* dest, const Long* self, ddigit n) {
  int i;
  digit *dd, *sd;
  const int d = (int)n / SHIFT_BIT;
  const int length = abs(self->l_);
  const ddigit b = n % SHIFT_BIT;
  const ddigit mask = (1 << b) - 1;

  if (length <= d) {
    longInit(dest);
    return;
  }

  longAlloc(dest, longIsNeg(self) ? -(length - d) : length - d);

  dd = dest->d_;
  sd = self->d_;
  for (i = 0; i < length - d - 1; ++i) {
    dd[i] = ((sd[i + d + 1] & mask) << (SHIFT_BIT - b)) + (sd[i + d] >> b);
  }
  dd[i] = sd[i + d] >> b;

  longNorm(dest);
}

void longSquare(Long* dest, const Long* self) {
  ddigit u, v ,uv, c;
  digit *dd, *sd;
  int i, j;
  const int length = abs(self->l_);

  longFillZero(dest, length << 1);

  dd = dest->d_;
  sd = self->d_;
  for (i = 0; i < length; ++i) {
    uv = dd[i << 1] + sd[i] * sd[i];
    u = uv >> SHIFT_BIT;
    v = uv & MASK;
    dd[i << 1] = (digit)v;
    c = u;
    for (j = i + 1; j < length; ++j) {
      uv = sd[j] * sd[i];
      u = (uv >> SHIFT_BIT) << 1;
      v = (uv & MASK) << 1;
      v += dd[i + j] + c;
      u += v >> SHIFT_BIT;
      v &= MASK;
      dd[i + j] = (digit)v;
      c = u;
    }
    dd[i + length] = (digit)u;
  }

  longNorm(dest);
}

/**
 * square root.
 * TODO: negative case
 */
void longSqrt(Long* dest, const Long* self) {
  Long c, t, d;
  longInit(&c);
  longInit(&t);
  longInit(&d);
  longNum(&c, 1);
  longAbs(dest, self);
  longAbs(&d, self);
  while (longGt(dest, &c)) {
    longRightShift(&t, dest, 1);
    longClone(dest, &t);

    longLeftShift(&t, &c, 1);
    longClone(&c, &t);
  }

  do {
    longClone(dest, &c);

    longDiv(&c, &d, dest);
    longAdd(&t, &c, dest);
    longRightShift(&c, &t, 1);
  } while (longGt(dest, &c));

  longFree(&c);
  longFree(&t);
  longFree(&d);
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

int longCmpAbs(const Long* lhs, const Long* rhs) {
  int al;
  digit *ld, *rd;

  if (lhs == rhs) { return 0; }
  al = abs(lhs->l_);
  if (al < abs(rhs->l_)) { return -1; }
  if (al > abs(rhs->l_)) { return 1; }
  if (al == 0) { return 0; }

  al = abs(al);
  ld = lhs->d_;
  rd = rhs->d_;
  do { --al; } while (al && ld[al] == rd[al]);
  return ld[al] > rd[al] ? 1 :
      ld[al] < rd[al] ? -1 : 0;
}

int longCmp(const Long* lhs, const Long* rhs) {
  int al;
  digit *ld, *rd;
  const int sign = lhs->l_ ^ rhs->l_;

  if (lhs == rhs) { return 0; }
  al = lhs->l_;
  if (al < rhs->l_) { return -1; }
  if (al > rhs->l_) { return 1; }
  if (al == 0) { return 0; }

  al = abs(al);
  ld = lhs->d_;
  rd = rhs->d_;
  do { --al; } while (al && ld[al] == rd[al]);
  return ld[al] > rd[al] ? (sign < 0 ? -1 : 1) :
      ld[al] < rd[al] ? (sign < 0 ? 1 : -1) : 0;
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
