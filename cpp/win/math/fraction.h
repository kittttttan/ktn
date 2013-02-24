#pragma once
#ifndef KTN_MATH_FRACTION_H_
#define KTN_MATH_FRACTION_H_

#include "ulong.h"
#include <string>
#include <iostream>

namespace ktn { namespace math {

/**
 * @brief Fraction
 */
class Fraction
{
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::istream& operator>>(std::istream& is, Fraction& f);

public:
    explicit Fraction() : s_(true), n_(0), d_(1) {}
    explicit Fraction(ddigit a);
    explicit Fraction(ddigit a, ddigit b);
    Fraction(const Fraction& f);
    explicit Fraction(const ULong& a, bool s=true) : s_(s), n_(a), d_(1) {}
    explicit Fraction(const ULong& a, const ULong& b,bool s=true);
    explicit Fraction(const char *s, int radix);
    ~Fraction(){}

    static const Fraction ZERO;
    static const Fraction ONE;

    ULong numerator() const { return n_; }
    ULong denominator() const { return d_; }
    bool sign() const { return s_; }

    std::string str(int radix=10) const;
    void cstr(char *s, int radix=10) const;
    void out(int radix=10, bool br=true) const;
    int cmp(const Fraction& b) const;
    int cmp(ddigit b) const;

    bool operator!() const;
    Fraction operator+() const;
    Fraction operator-() const;
    Fraction& operator++();
    Fraction operator++(int);
    Fraction& operator--();
    Fraction operator--(int);

    Fraction operator+(const Fraction& b) const;
    Fraction operator-(const Fraction& b) const;
    Fraction operator*(const Fraction& b) const;
    Fraction operator/(const Fraction& b) const;

    Fraction operator+(ddigit b) const;
    Fraction operator-(ddigit b) const;
    Fraction operator*(ddigit b) const;
    Fraction operator/(ddigit b) const;
    Fraction operator<<(ddigit n) const;
    Fraction operator>>(ddigit n) const;

    Fraction& operator=(const Fraction& b);
    Fraction& operator+=(const Fraction& b);
    Fraction& operator-=(const Fraction& b);
    Fraction& operator*=(const Fraction& b);
    Fraction& operator/=(const Fraction& b);

    Fraction& operator+=(ddigit b);
    Fraction& operator-=(ddigit b);
    Fraction& operator*=(ddigit b);
    Fraction& operator/=(ddigit b);
    Fraction& operator<<=(ddigit n);
    Fraction& operator>>=(ddigit n);

    bool operator==(const Fraction& b) const;
    bool operator!=(const Fraction& b) const;
    bool operator<(const Fraction& b) const;
    bool operator>(const Fraction& b) const;
    bool operator<=(const Fraction& b) const;
    bool operator>=(const Fraction& b) const;

    bool operator==(ddigit b) const;
    bool operator!=(ddigit b) const;
    bool operator<(ddigit b) const;
    bool operator>(ddigit b) const;
    bool operator<=(ddigit b) const;
    bool operator>=(ddigit b) const;

private:
    void cancel();

    bool s_;  /**< sign */
    ULong n_;  /**< numerator */
    ULong d_;  /**< denominator */
};
}} // namespace ktn math
#endif // KTN_MATH_FRACTION_H_
