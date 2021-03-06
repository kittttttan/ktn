#pragma once
#ifndef KTN_MATH_FRACTION_H_
#define KTN_MATH_FRACTION_H_

#include "ulong.h"
#include <string>
#include <iostream>

#include <stdio.h>
#include <sstream>
#include <stdexcept>

namespace ktn { namespace math {

class Fraction;

template<class Dummy>
struct Fraction_constants_
{
public:
	static const Fraction ZERO;
	static const Fraction ONE;
};

template<class Dummy>
const Fraction Fraction_constants_<Dummy>::ZERO = Fraction(0);

template<class Dummy>
const Fraction Fraction_constants_<Dummy>::ONE = Fraction(1);

typedef Fraction_constants_<void> Fraction_constants;

/*!
 @brief Fraction
 */
class Fraction : public Fraction_constants
{
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::istream& operator>>(std::istream& is, Fraction& f);

public:
    static const Fraction ZERO;
    static const Fraction ONE;

public:
    explicit Fraction() : s_(true), n_(0), d_(1) {}
    explicit Fraction(ddigit a);
    explicit Fraction(ddigit a, ddigit b);
    Fraction(const Fraction& f);
    explicit Fraction(const ULong& a, bool s=true) : s_(s), n_(a), d_(1) {}
    explicit Fraction(const ULong& a, const ULong& b,bool s=true);
    explicit Fraction(const char *s, int radix);
    ~Fraction(){}

    ULong numerator() const { return n_; }
    ULong denominator() const { return d_; }
    bool sign() const { return s_; }

    std::string str(int radix=10) const;
    //void cstr(char *s, int radix=10) const;
    void out(int radix=10, bool br=true) const;
    inline int cmp(const Fraction& b) const;
    inline int cmp(ddigit b) const;

    inline bool operator!() const;
    inline Fraction operator+() const;
    inline Fraction operator-() const;
    inline Fraction& operator++();
    inline Fraction operator++(int);
    inline Fraction& operator--();
    inline Fraction operator--(int);

    inline Fraction operator+(const Fraction& b) const;
    inline Fraction operator-(const Fraction& b) const;
    inline Fraction operator*(const Fraction& b) const;
    inline Fraction operator/(const Fraction& b) const;

    inline Fraction operator+(ddigit b) const;
    inline Fraction operator-(ddigit b) const;
    inline Fraction operator*(ddigit b) const;
    inline Fraction operator/(ddigit b) const;
    inline Fraction operator<<(ddigit n) const;
    inline Fraction operator>>(ddigit n) const;

    inline Fraction& operator=(const Fraction& b);
    inline Fraction& operator+=(const Fraction& b);
    inline Fraction& operator-=(const Fraction& b);
    inline Fraction& operator*=(const Fraction& b);
    inline Fraction& operator/=(const Fraction& b);

    inline Fraction& operator+=(ddigit b);
    inline Fraction& operator-=(ddigit b);
    inline Fraction& operator*=(ddigit b);
    inline Fraction& operator/=(ddigit b);
    inline Fraction& operator<<=(ddigit n);
    inline Fraction& operator>>=(ddigit n);

    inline bool operator==(const Fraction& b) const;
    inline bool operator!=(const Fraction& b) const;
    inline bool operator<(const Fraction& b) const;
    inline bool operator>(const Fraction& b) const;
    inline bool operator<=(const Fraction& b) const;
    inline bool operator>=(const Fraction& b) const;

    inline bool operator==(ddigit b) const;
    inline bool operator!=(ddigit b) const;
    inline bool operator<(ddigit b) const;
    inline bool operator>(ddigit b) const;
    inline bool operator<=(ddigit b) const;
    inline bool operator>=(ddigit b) const;

private:
    void cancel();

    /*!
     sign
     */
    bool s_;

    /*!
     numerator
     */
    ULong n_;

    /*!
     denominator
     */
    ULong d_;
};
}} // namespace ktn math

#include "fraction.inl"

#endif // KTN_MATH_FRACTION_H_
