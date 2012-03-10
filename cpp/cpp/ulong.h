#ifndef ULONG_H_
#define ULONG_H_

#include <string>
#include <iostream>

#define USE_LONGLONG
#ifdef USE_LONGLONG
typedef long long BitSize;
#else
typedef long BitSize;
#endif

namespace mathktn {
class ULong {
friend std::ostream& operator<<(std::ostream& os, ULong l);

public:
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
	ULong operator<<(BitSize n) const;
	ULong operator>>(BitSize n) const;

	ULong& operator=(const ULong& b);
	ULong& operator+=(const ULong& b);
	ULong& operator-=(const ULong& b);
	ULong& operator*=(const ULong& b);
	ULong& operator/=(const ULong& b);
	ULong& operator%=(const ULong& b);
	ULong& operator<<=(BitSize n);
	ULong& operator>>=(BitSize n);

	bool operator==(const ULong& b) const;
	bool operator!=(const ULong& b) const;
	bool operator<(const ULong& b) const;
	bool operator>(const ULong& b) const;
	bool operator<=(const ULong& b) const;
	bool operator>=(const ULong& b) const;

private:
	void alloc(int length, bool zero);
	void norm();

	int l_;
	BitSize* d_;
};
} // namespace mathktn
#endif // ULONG_H_
