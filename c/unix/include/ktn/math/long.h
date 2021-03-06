#pragma once

#include <stdbool.h>
#include <stdint.h>

/*
#define USE_64BIT
*/
#ifdef USE_64BIT
typedef int32_t digit;
typedef int64_t ddigit;
#else
typedef int16_t digit;
typedef int32_t ddigit;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Long {
  uint32_t a_; /**< allocate size */
  int32_t l_;  /**< length */
  digit* d_;   /**< digits */
} Long;

void longInit(Long* self);
void longAlloc(Long* self, int32_t length);
void longNum(Long* self, ddigit n);
void longFree(Long* self);

void longClone(Long* dest, const Long* src);
void longAbs(Long* dest, const Long* src);
void longNeg(Long* dest, const Long* src);

void longWrite(const Long* self);
void longWriteln(const Long* self);
void longStr(const Long* self, char *s);

void longAddAbs(Long* dest, const Long* lhs, const Long* rhs);
void longSubAbs(Long* dest, const Long* lhs, const Long* rhs);
void longAdd(Long* dest, const Long* lhs, const Long* rhs);
void longSub(Long* dest, const Long* lhs, const Long* rhs);
void longMul(Long* dest, const Long* lhs, const Long* rhs);
void longDivmod(Long* dest, const Long* lhs, const Long* rhs, bool mod);

int longCmp(const Long* lhs, const Long* rhs);
int longCmpAbs(const Long* lhs, const Long* rhs);
bool longEq(const Long* lhs, const Long* rhs);
bool longNe(const Long* lhs, const Long* rhs);
bool longLt(const Long* lhs, const Long* rhs);
bool longGt(const Long* lhs, const Long* rhs);
bool longLe(const Long* lhs, const Long* rhs);
bool longGe(const Long* lhs, const Long* rhs);

void longLeftShift(Long* dest, const Long* self, ddigit n);
void longRightShift(Long* dest, const Long* self, ddigit n);

void longSquare(Long* dest, const Long* self);
void longSqrt(Long* dest, const Long* self);
void longPow(Long* dest, const Long* self, ddigit n);

#define longIsZero(self)        ((self)->l_ == 0)
#define longIsOne(self)         ((self)->l_ == 1 && (self)->d_[0] == 1)
#define longIsNeg(self)         ((self)->l_ < 0)
#define longDiv(dest, lhs, rhs) longDivmod(dest, lhs, rhs, false)
#define longMod(dest, lhs, rhs) longDivmod(dest, lhs, rhs, true)

#ifdef __cplusplus
}
#endif
