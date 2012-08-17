#pragma once
#ifndef KTN_MATH_ULONG_H_
#define KTN_MATH_ULONG_H_

/**
* @file  math/ulong.h
* @brief ULong
*/

#include <string>
#include <iostream>

namespace ktn { namespace math {

//#define USE_64BIT
#ifdef USE_64BIT
typedef long digit;
typedef long long ddigit;
#else
typedef short digit;
typedef long ddigit;
#endif

/**
* @brief Unsigned BigInteger.
*/
class ULong {

    friend std::ostream& operator<<(std::ostream& os, const ULong& l);
    //friend std::istream& operator>>(std::istream& is, ULong& l);

public:
    explicit ULong() : l_(0), d_(nullptr) {}
    explicit ULong(ddigit u);
    ULong(const ULong& l);
    explicit ULong(const char *s, int base);
    ~ULong() { delete [] d_; }

    static const ULong ZERO;
    static const ULong ONE;

    static ULong random(int n);

    std::string str(int base=10) const;
    void cstr(char *s, int base=10) const;
    void out(int base=10, bool br=true) const;
    void debug() const;

    ULong square() const;
    ULong sqrt() const;
    ULong pow(ddigit n) const;
    ULong karatsuba(const ULong& u) const;
    ULong divmod(const ULong& b, bool mod) const;
    ULong gcd(const ULong& b) const;
    ULong gcdBin(const ULong& b) const;
    ddigit bitLength() const;
    int cmp(const ULong& b) const;
    int cmp(ddigit b) const;

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

    inline ULong operator+(ddigit b) const;
    inline ULong operator-(ddigit b) const;
    inline ULong operator*(ddigit b) const;
    inline ULong operator/(ddigit b) const;
    inline ULong operator%(ddigit b) const;
    ULong operator<<(ddigit n) const;
    ULong operator>>(ddigit n) const;

    ULong& operator=(const ULong& b);
    inline ULong& operator+=(const ULong& b);
    inline ULong& operator-=(const ULong& b);
    inline ULong& operator*=(const ULong& b);
    inline ULong& operator/=(const ULong& b);
    inline ULong& operator%=(const ULong& b);

    inline ULong& operator+=(ddigit b);
    inline ULong& operator-=(ddigit b);
    inline ULong& operator*=(ddigit b);
    inline ULong& operator/=(ddigit b);
    inline ULong& operator%=(ddigit b);
    inline ULong& operator<<=(ddigit n);
    inline ULong& operator>>=(ddigit n);

    inline bool operator==(const ULong& b) const;
    inline bool operator!=(const ULong& b) const;
    inline bool operator<(const ULong& b) const;
    inline bool operator>(const ULong& b) const;
    inline bool operator<=(const ULong& b) const;
    inline bool operator>=(const ULong& b) const;

    inline bool operator==(ddigit b) const;
    inline bool operator!=(ddigit b) const;
    inline bool operator<(ddigit b) const;
    inline bool operator>(ddigit b) const;
    inline bool operator<=(ddigit b) const;
    inline bool operator>=(ddigit b) const;

private:
    void alloc(int length, bool zero);
    inline void norm();

    int l_;      /**< length */
    digit* d_;  /**< digit blocks */
};

}} // namespace ktn math

#include "ulong.inl"

#endif // KTN_MATH_ULONG_H_
