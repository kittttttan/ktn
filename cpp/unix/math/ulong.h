#pragma once
#ifndef KTN_MATH_ULONG_H_
#define KTN_MATH_ULONG_H_

#include <cstdint>
#include <string>
#include <iostream>

namespace ktn { namespace math {

//#define USE_64BIT
#ifdef USE_64BIT
static_assert(false, "FIXME: USE_64BIT mode is buggy");
typedef int32_t digit;
typedef int64_t ddigit;
typedef uint32_t udigit;
typedef uint64_t uddigit;
#else
typedef int16_t digit;
typedef int32_t ddigit;
typedef uint16_t udigit;
typedef uint32_t uddigit;
#endif

/*!
 @brief Unsigned BigInteger.
 */
class ULong
{
    friend std::ostream& operator<<(std::ostream& os, const ULong& l);
    friend std::istream& operator>>(std::istream& is, ULong& l);

public:
    static const ULong ZERO;
    static const ULong ONE;

    static ULong random(size_t n);
    static ULong factorial(uddigit n);
    static ULong fact_odd(uddigit n);
    static ULong fact_even(uddigit n);

public:
    explicit ULong() : l_(0), d_(nullptr) {}
    explicit ULong(uddigit u);
    ULong(const ULong& l);
    explicit ULong(const char *s, int base);
    ~ULong() { delete [] d_; }

    size_t capacity() const { return c_; }
    size_t length() const { return l_; }

    bool isOdd() const { return l_ > 0 && (d_[0] & 1) == 1; }
    bool isEven() const { return l_ < 1 || (d_[0] & 1) == 0; }

    std::string str(int base=10) const;
    void cstr(char *s, int base=10) const;
    void out(int base=10, bool br=true) const;
    void debug() const;

    ULong square() const;
    ULong sqrt() const;
    ULong pow(uddigit n) const;
    ULong karatsuba(const ULong& u) const;
    ULong divmod(const ULong& b, bool mod) const;
    ULong gcd(const ULong& b) const;
    ULong gcdBin(const ULong& b) const;
    uddigit bitLength() const;
    int cmp(const ULong& b) const;
    int cmp(uddigit b) const;

    //explicit operator bool() const { return d_ && d_[0] > 0); }

    inline bool operator!() const;
    inline ULong operator+() const;
    inline ULong& operator++();
    inline ULong operator++(int);
    inline ULong& operator--();
    inline ULong operator--(int);

    ULong operator+(const ULong& b) const;
    ULong operator-(const ULong& b) const;
    ULong operator*(const ULong& b) const;
    inline ULong operator/(const ULong& b) const;
    inline ULong operator%(const ULong& b) const;

    inline ULong operator+(uddigit b) const;
    inline ULong operator-(uddigit b) const;
    inline ULong operator*(uddigit b) const;
    inline ULong operator/(uddigit b) const;
    inline ULong operator%(uddigit b) const;
    ULong operator<<(uddigit n) const;
    ULong operator>>(uddigit n) const;

    ULong& operator=(const ULong& b);
    inline ULong& operator+=(const ULong& b);
    inline ULong& operator-=(const ULong& b);
    inline ULong& operator*=(const ULong& b);
    inline ULong& operator/=(const ULong& b);
    inline ULong& operator%=(const ULong& b);

    inline ULong& operator+=(uddigit b);
    inline ULong& operator-=(uddigit b);
    inline ULong& operator*=(uddigit b);
    inline ULong& operator/=(uddigit b);
    inline ULong& operator%=(uddigit b);
    inline ULong& operator<<=(uddigit n);
    inline ULong& operator>>=(uddigit n);

    inline bool operator==(const ULong& b) const;
    inline bool operator!=(const ULong& b) const;
    inline bool operator<(const ULong& b) const;
    inline bool operator>(const ULong& b) const;
    inline bool operator<=(const ULong& b) const;
    inline bool operator>=(const ULong& b) const;

    inline bool operator==(uddigit b) const;
    inline bool operator!=(uddigit b) const;
    inline bool operator<(uddigit b) const;
    inline bool operator>(uddigit b) const;
    inline bool operator<=(uddigit b) const;
    inline bool operator>=(uddigit b) const;

private:
    static const uddigit SHIFT_BIT;
    static const uddigit BASE;
    static const uddigit MASK;
    static const char FORMAT_DIGIT[];
    static const char FORMAT_DDIGIT[];
    static const char FORMAT_B[];

private:
    void alloc(size_t length, bool zero);
    inline void norm();

    /*!
     capacity
     */
    size_t c_;

    /*!
     length
     */
    size_t l_;

    /*!
     digit blocks
     */
    udigit* d_;
};

}} // namespace ktn math

#include "ulong.inl"

#endif // KTN_MATH_ULONG_H_
