#ifndef KTN_MATH_FRACTION_H_
#define KTN_MATH_FRACTION_H_

/**
 * @file  math/fraction.h
 * @brief Fraction
 */

#include <string>
#include <iostream>
#include "ulong.h"

namespace ktn { namespace math {

/**
 * @brief Fraction
 */
class Fraction {

friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
//friend std::istream& operator>>(std::istream& is, Fraction& f);

public:
	explicit Fraction() : s_(true), n_(0), d_(1) {}
	explicit Fraction(BitSize a);
	explicit Fraction(BitSize a, BitSize b);
	Fraction(const Fraction& f);
	explicit Fraction(const ULong& a, bool s=true) : s_(s), n_(a), d_(1) {}
	explicit Fraction(const ULong& a, const ULong& b,bool s=true);
	explicit Fraction(const char *s, int radix);
	~Fraction(){}

	static const Fraction ZERO;
	static const Fraction ONE;

    ULong numerator() const { return n_; }
    ULong denominator() const { return d_; }
	bool sign() { return s_; }

	std::string str(int radix=10) const;
	void cstr(char *s, int radix=10) const;
	void out(int radix=10, bool br=true) const;
	int cmp(const Fraction& b) const;
	int cmp(BitSize b) const;

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

	Fraction operator+(BitSize b) const;
	Fraction operator-(BitSize b) const;
	Fraction operator*(BitSize b) const;
	Fraction operator/(BitSize b) const;
	Fraction operator<<(BitSize n) const;
	Fraction operator>>(BitSize n) const;

	Fraction& operator=(const Fraction& b);
	Fraction& operator+=(const Fraction& b);
	Fraction& operator-=(const Fraction& b);
	Fraction& operator*=(const Fraction& b);
	Fraction& operator/=(const Fraction& b);

	Fraction& operator+=(BitSize b);
	Fraction& operator-=(BitSize b);
	Fraction& operator*=(BitSize b);
	Fraction& operator/=(BitSize b);
	Fraction& operator<<=(BitSize n);
	Fraction& operator>>=(BitSize n);

	bool operator==(const Fraction& b) const;
	bool operator!=(const Fraction& b) const;
	bool operator<(const Fraction& b) const;
	bool operator>(const Fraction& b) const;
	bool operator<=(const Fraction& b) const;
	bool operator>=(const Fraction& b) const;

	bool operator==(BitSize b) const;
	bool operator!=(BitSize b) const;
	bool operator<(BitSize b) const;
	bool operator>(BitSize b) const;
	bool operator<=(BitSize b) const;
	bool operator>=(BitSize b) const;

private:
	void cancel();

	bool s_;	/**< sign */
	ULong n_;	/**< numerator */
	ULong d_;	/**< denominator */
};
}} // namespace ktn math
#endif // KTN_MATH_FRACTION_H_
