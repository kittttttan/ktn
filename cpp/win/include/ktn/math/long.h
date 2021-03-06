﻿#pragma once
#ifndef KTN_MATH_LONG_H_
#define KTN_MATH_LONG_H_

#include "ktn/math/ulong.h"
#include <string>
#include <iostream>

namespace ktn { namespace math {

class Long;

template<class Dummy>
struct Long_constants_
{
public:
	static const Long ZERO;
	static const Long ONE;
};

template<class Dummy>
const Long Long_constants_<Dummy>::ZERO = Long("", 10);

template<class Dummy>
const Long Long_constants_<Dummy>::ONE = Long(1);

typedef Long_constants_<void> Long_constants;

/*!
 @brief Signed BigInteger.
 */
class Long : public Long_constants
{
    friend std::ostream& operator<<(std::ostream& os, const Long& l);
    friend std::istream& operator>>(std::istream &is, Long& l);

public:
    Long() : u_(), s_(true) {}
    Long(const Long& l);
    explicit Long(ddigit a);
    explicit Long(const ULong& u, bool s=true) : u_(u), s_(s) {}
    explicit Long(const char *s, int base);
    ~Long() {}

    bool sign() const { return s_; }

    void out(int base=10) const;
    void cstr(char *s, int base=10) const;
    std::string str(int base=10) const;
    void debug() const;
    inline int cmp(const Long& b) const;
    inline int cmp(ddigit b) const;

    bool operator!() const;
    Long operator+() const;
    Long operator-() const;
    Long& operator++();
    Long operator++(int);
    Long& operator--();
    Long operator--(int);

    inline Long operator+(const Long& b) const;
    inline Long operator-(const Long& b) const;
    inline Long operator*(const Long& b) const;
    inline Long operator/(const Long& b) const;
    inline Long operator%(const Long& b) const;

    inline Long operator+(ddigit b) const;
    inline Long operator-(ddigit b) const;
    inline Long operator*(ddigit b) const;
    inline Long operator/(ddigit b) const;
    inline Long operator%(ddigit b) const;
    inline Long operator<<(ddigit n) const;
    inline Long operator>>(ddigit n) const;

    inline Long& operator=(const Long& b);
    inline Long& operator+=(const Long& b);
    inline Long& operator-=(const Long& b);
    inline Long& operator*=(const Long& b);
    inline Long& operator/=(const Long& b);
    inline Long& operator%=(const Long& b);

    inline Long& operator+=(ddigit b);
    inline Long& operator-=(ddigit b);
    inline Long& operator*=(ddigit b);
    inline Long& operator/=(ddigit b);
    inline Long& operator%=(ddigit b);
    inline Long& operator<<=(ddigit n);
    inline Long& operator>>=(ddigit n);

    inline bool operator==(const Long& b) const;
    inline bool operator!=(const Long& b) const;
    inline bool operator<(const Long& b) const;
    inline bool operator>(const Long& b) const;
    inline bool operator<=(const Long& b) const;
    inline bool operator>=(const Long& b) const;

    inline bool operator==(ddigit b) const;
    inline bool operator!=(ddigit b) const;
    inline bool operator<(ddigit b) const;
    inline bool operator>(ddigit b) const;
    inline bool operator<=(ddigit b) const;
    inline bool operator>=(ddigit b) const;

private:
    ULong u_;

    /*!
     sign
     */
    bool s_;
};
}} // namespace ktn math

#include "ktn/math/long.inl"

#endif // KTN_MATH_LONG_H_
