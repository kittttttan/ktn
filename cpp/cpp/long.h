#ifndef LONG_H_
#define LONG_H_

#include <string>
#include <iostream>
#include "ulong.h"

namespace mathktn {
class Long {
friend std::ostream& operator<<(std::ostream& os, Long l);

public:
	void out(int base=10);
	void cstr(char *s, int base=10);
	std::string str(int base=10);
	void debug();
	int cmp(const Long& b) const;

	Long();
	Long(const Long& l);
	explicit Long(BitSize a);
	explicit Long(const ULong& u, bool s=true);
	explicit Long(const char *s, int base);
	~Long();

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
	Long operator<<(BitSize n) const;
	Long operator>>(BitSize n) const;

	Long& operator=(const Long& b);
	Long& operator+=(const Long& b);
	Long& operator-=(const Long& b);
	Long& operator*=(const Long& b);
	Long& operator/=(const Long& b);
	Long& operator%=(const Long& b);
	Long& operator<<=(BitSize n);
	Long& operator>>=(BitSize n);

	bool operator==(const Long& b) const;
	bool operator!=(const Long& b) const;
	bool operator<(const Long& b) const;
	bool operator>(const Long& b) const;
	bool operator<=(const Long& b) const;
	bool operator>=(const Long& b) const;

private:
	ULong u_;
	bool s_;
};
} // namespace mathktn
#endif // LONG_H_
