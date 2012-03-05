#ifndef FRACTION_H_
#define FRACTION_H_

#include <cstdint>
#include <string>
#include <iostream>
#include "ulong.h"

namespace mathktn {
class Fraction {
friend std::ostream& operator<<(std::ostream& os, Fraction f);

public:
	std::string str(int base=10);
	void cstr(char *s, int base=10);
	void out(int base=10, bool br=true);
	int cmp(const Fraction& b) const;

	explicit Fraction();
	explicit Fraction(int64_t a);
	explicit Fraction(int64_t a, int64_t b);
	Fraction(const Fraction& f);
	explicit Fraction(const ULong& a, bool s=true);
	explicit Fraction(const ULong& a, const ULong& b,bool s=true);
	explicit Fraction(const char *s, int base);
	~Fraction();

	bool operator!() const;
	Fraction& operator++();
	Fraction operator++(int);
	Fraction& operator--();
	Fraction operator--(int);

	Fraction operator+(const Fraction& b) const;
	Fraction operator-(const Fraction& b) const;
	Fraction operator*(const Fraction& b) const;
	Fraction operator/(const Fraction& b) const;
	Fraction operator<<(int64_t n) const;
	Fraction operator>>(int64_t n) const;

	Fraction& operator=(const Fraction& b);
	Fraction& operator+=(const Fraction& b);
	Fraction& operator-=(const Fraction& b);
	Fraction& operator*=(const Fraction& b);
	Fraction& operator/=(const Fraction& b);
	Fraction& operator<<=(int64_t n);
	Fraction& operator>>=(int64_t n);

	bool operator==(const Fraction& b) const;
	bool operator!=(const Fraction& b) const;
	bool operator<(const Fraction& b) const;
	bool operator>(const Fraction& b) const;
	bool operator<=(const Fraction& b) const;
	bool operator>=(const Fraction& b) const;

private:
	void cancel();

	bool s_;
	ULong n_;
	ULong d_;
};
} // namespace mathktn
#endif // FRACTION_H_
