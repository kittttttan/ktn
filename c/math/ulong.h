#ifndef CKTN_MATH_ULONG_H_
#define CKTN_MATH_ULONG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../bool.h"

//#define USE_64BIT
#ifdef USE_64BIT
typedef long digit;
typedef long long ddigit;
#else
typedef short digit;
typedef long ddigit;
#endif
typedef struct _ULong {
	int l_;
	digit* d_;
} ULong;

void ulongInit(ULong* self);
void ulongAlloc(ULong* self, int length);
void ulongNum(ULong* self, ddigit n);
void ulongFree(ULong* self);

void ulongClone(ULong* dest, const ULong* src);

void ulongWrite(const ULong* self);
void ulongWriteln(const ULong* self);
void ulongStr(const ULong* self, char *s);

void ulongAdd(ULong* dest, const ULong* lhs, const ULong* rhs);
void ulongSub(ULong* dest, const ULong* lhs, const ULong* rhs);
void ulongMul(ULong* dest, const ULong* lhs, const ULong* rhs);
void ulongDivmod(ULong* dest, const ULong* lhs, const ULong* rhs, bool mod);

int ulongCmp(const ULong* lhs, const ULong* rhs);
bool ulongEq(const ULong* lhs, const ULong* rhs);
bool ulongNe(const ULong* lhs, const ULong* rhs);
bool ulongLt(const ULong* lhs, const ULong* rhs);
bool ulongGt(const ULong* lhs, const ULong* rhs);
bool ulongLe(const ULong* lhs, const ULong* rhs);
bool ulongGe(const ULong* lhs, const ULong* rhs);

void ulongLeftShift(ULong* dest, const ULong* self, ddigit n);
void ulongRightShift(ULong* dest, const ULong* self, ddigit n);

void ulongSquare(ULong* dest, const ULong* self);
void ulongSqrt(ULong* dest, const ULong* self);
void ulongPow(ULong* dest, const ULong* self, ddigit n);

#define ulongIsZero(self)			(self->l_ < 1)
#define ulongIsOne(self)			(self->l_ == 1 && self->d_[0] == 1)
#define ulongDiv(dest, lhs, rhs)	ulongDivmod(dest, lhs, rhs, false)
#define ulongMod(dest, lhs, rhs)	ulongDivmod(dest, lhs, rhs, true)

#ifdef __cplusplus
}
#endif
#endif // CKTN_MATH_ULONG_H_
