#ifndef MATHKTN_ULONG_H_
#define MATHKTN_ULONG_H_

#include <string>
#include <iostream>

namespace mathktn {
typedef long long BitSize;

class ULong {
friend std::ostream& operator<<(std::ostream& os, ULong l);

public:
	static const ULong ZERO;
	static const ULong ONE;

	std::string str(int base=10);
	void cstr(char *s, int base=10);
	void out(int base=10, bool br=true);
	void debug();

	static ULong random(int n);

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

	explicit ULong();
	explicit ULong(BitSize u);
	ULong(const ULong& l);
	explicit ULong(const char *s, int base);
	~ULong();

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

	int l_;
	BitSize* d_;
};
} // namespace mathktn
#endif // MATHKTN_ULONG_H_
