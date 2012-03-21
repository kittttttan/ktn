#ifndef KTN_MATH_ULONG_H_
#define KTN_MATH_ULONG_H_

/**
 * @file  math/ulong.h
 * @brief ULong
 */

#include <string>
#include <iostream>

namespace ktn { namespace math {

typedef long long BitSize;

/**
 * @brief Unsigned BigInteger.
 */
class ULong {

friend std::ostream& operator<<(std::ostream& os, const ULong& l);
//friend std::istream& operator>>(std::istream& is, ULong& l);

public:
	explicit ULong() : l_(0), d_(nullptr) {}
	explicit ULong(BitSize u);
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
	ULong pow(BitSize n) const;
	ULong karatsuba(const ULong& u) const;
	ULong divmod(const ULong& b, bool mod) const;
	ULong gcd(const ULong& b) const;
	ULong gcdBin(const ULong& b) const;
	BitSize bitLength() const;
	int cmp(const ULong& b) const;
	int cmp(BitSize b) const;

	bool operator!() const;
	ULong operator+() const;
	ULong& operator++();
	ULong operator++(int);
	ULong& operator--();
	ULong operator--(int);

	ULong operator+(const ULong& b) const;
	ULong operator-(const ULong& b) const;
	ULong operator*(const ULong& b) const;
	ULong operator/(const ULong& b) const;
	ULong operator%(const ULong& b) const;

	ULong operator+(BitSize b) const;
	ULong operator-(BitSize b) const;
	ULong operator*(BitSize b) const;
	ULong operator/(BitSize b) const;
	ULong operator%(BitSize b) const;
	ULong operator<<(BitSize n) const;
	ULong operator>>(BitSize n) const;

	ULong& operator=(const ULong& b);
	ULong& operator+=(const ULong& b);
	ULong& operator-=(const ULong& b);
	ULong& operator*=(const ULong& b);
	ULong& operator/=(const ULong& b);
	ULong& operator%=(const ULong& b);

	ULong& operator+=(BitSize b);
	ULong& operator-=(BitSize b);
	ULong& operator*=(BitSize b);
	ULong& operator/=(BitSize b);
	ULong& operator%=(BitSize b);
	ULong& operator<<=(BitSize n);
	ULong& operator>>=(BitSize n);

	bool operator==(const ULong& b) const;
	bool operator!=(const ULong& b) const;
	bool operator<(const ULong& b) const;
	bool operator>(const ULong& b) const;
	bool operator<=(const ULong& b) const;
	bool operator>=(const ULong& b) const;

	bool operator==(BitSize b) const;
	bool operator!=(BitSize b) const;
	bool operator<(BitSize b) const;
	bool operator>(BitSize b) const;
	bool operator<=(BitSize b) const;
	bool operator>=(BitSize b) const;

private:
	void alloc(int length, bool zero);
	void norm();

	int l_;			/**< length */
	BitSize* d_;	/**< digit blocks */
};
}} // namespace ktn math
#endif // KTN_MATH_ULONG_H_
