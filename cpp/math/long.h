#ifndef KTN_MATH_LONG_H_
#define KTN_MATH_LONG_H_

/**
 * @file  math/long.h
 * @brief Long
 */

#include <string>
#include <iostream>
#include "ulong.h"

namespace ktn { namespace math {

/**
 * @brief Signed BigInteger.
 */
class Long {

friend std::ostream& operator<<(std::ostream& os, const Long& l);
//friend std::istream& operator>>(std::istream &is, Long& l);

public:
	Long() : u_(), s_(true) {}
	Long(const Long& l);
	explicit Long(BitSize a);
	explicit Long(const ULong& u, bool s=true) : u_(u), s_(s) {}
	explicit Long(const char *s, int base);
	~Long() {}

	static const Long ZERO;
	static const Long ONE;

	bool sign() { return s_; }

	void out(int base=10) const;
	void cstr(char *s, int base=10) const;
	std::string str(int base=10) const;
	void debug() const;
	int cmp(const Long& b) const;
	int cmp(BitSize b) const;

	bool operator!() const;
	Long operator+() const;
	Long operator-() const;
	Long& operator++();
	Long operator++(int);
	Long& operator--();
	Long operator--(int);

	Long operator+(const Long& b) const;
	Long operator-(const Long& b) const;
	Long operator*(const Long& b) const;
	Long operator/(const Long& b) const;
	Long operator%(const Long& b) const;

	Long operator+(BitSize b) const;
	Long operator-(BitSize b) const;
	Long operator*(BitSize b) const;
	Long operator/(BitSize b) const;
	Long operator%(BitSize b) const;
	Long operator<<(BitSize n) const;
	Long operator>>(BitSize n) const;

	Long& operator=(const Long& b);
	Long& operator+=(const Long& b);
	Long& operator-=(const Long& b);
	Long& operator*=(const Long& b);
	Long& operator/=(const Long& b);
	Long& operator%=(const Long& b);

	Long& operator+=(BitSize b);
	Long& operator-=(BitSize b);
	Long& operator*=(BitSize b);
	Long& operator/=(BitSize b);
	Long& operator%=(BitSize b);
	Long& operator<<=(BitSize n);
	Long& operator>>=(BitSize n);

	bool operator==(const Long& b) const;
	bool operator!=(const Long& b) const;
	bool operator<(const Long& b) const;
	bool operator>(const Long& b) const;
	bool operator<=(const Long& b) const;
	bool operator>=(const Long& b) const;

	bool operator==(BitSize b) const;
	bool operator!=(BitSize b) const;
	bool operator<(BitSize b) const;
	bool operator>(BitSize b) const;
	bool operator<=(BitSize b) const;
	bool operator>=(BitSize b) const;

private:
	ULong u_;
	bool s_;	/**< sign */
};
}} // namespace ktn math
#endif // KTN_MATH_LONG_H_
