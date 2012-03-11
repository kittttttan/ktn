#ifndef FRACTION_H_
#define FRACTION_H_

#include <string>
#include <iostream>
#include "ulong.h"

namespace mathktn {
class Fraction {
friend std::ostream& operator<<(std::ostream& os, Fraction f);

public:
	explicit Fraction() : s_(true), n_(0), d_(1) {}
	explicit Fraction(BitSize a);
	explicit Fraction(BitSize a, BitSize b);
	Fraction(const Fraction& f);
	explicit Fraction(const ULong& a, bool s=true) : s_(s), n_(a), d_(1) {}
	explicit Fraction(const ULong& a, const ULong& b,bool s=true);
	explicit Fraction(const char *s, int base);
	~Fraction(){}

	static const Fraction ZERO;
	static const Fraction ONE;

    ULong numerator() const { return n_; }
    ULong denominator() const { return d_; }
	bool sign() { return s_; }

	std::string str(int base=10);
	void cstr(char *s, int base=10);
	void out(int base=10, bool br=true);
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

	bool s_;
	ULong n_;
	ULong d_;
};
} // namespace mathktn
#endif // FRACTION_H_
