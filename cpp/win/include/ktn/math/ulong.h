#pragma once
#ifndef KTN_MATH_ULONG_H_
#define KTN_MATH_ULONG_H_

#include <stdint.h>
#include <string>
#include <iostream>

namespace ktn { namespace math {

//#define USE_64BIT
#ifdef USE_64BIT
static_assert(false, "FIXME: USE_64BIT mode is buggy");
typedef int32_t digit;
typedef int64_t ddigit;
typedef uint32_t udigit;
typedef uint64_t uddigit;
#else
typedef int16_t digit;
typedef int32_t ddigit;
typedef uint16_t udigit;
typedef uint32_t uddigit;
#endif

class ULong;

template<class Dummy>
struct ULong_constants_
{
public:
	static const ULong ZERO;
	static const ULong ONE;
protected:
	static const uddigit SHIFT_BIT;
	static const uddigit BASE;
	static const uddigit MASK;
	static const char FORMAT_DIGIT[];
	static const char FORMAT_DDIGIT[];
	static const char FORMAT_B[];
};

#ifdef USE_64BIT
const uddigit ULong::SHIFT_BIT = 32;
const char ULong::FORMAT_DIGIT[] = "%lu";
const char ULong::FORMAT_DDIGIT[] = "%llu";
//const char ULong::FORMAT_DDIGIT[] = "%I64u";
#else
template<class Dummy>
const uddigit ULong_constants_<Dummy>::SHIFT_BIT = 16;
template<class Dummy>
const char ULong_constants_<Dummy>::FORMAT_DIGIT[] = "%u";
template<class Dummy>
const char ULong_constants_<Dummy>::FORMAT_DDIGIT[] = "%u";
#endif

template<class Dummy>
const char ULong_constants_<Dummy>::FORMAT_B[] = "%04x";

template<class Dummy>
const uddigit ULong_constants_<Dummy>::BASE = static_cast<uddigit>(1) << SHIFT_BIT;
template<class Dummy>
const uddigit ULong_constants_<Dummy>::MASK = BASE - 1;

/*!
0
*/
template<class Dummy>
const ULong ULong_constants_<Dummy>::ZERO = ULong();

/*!
1
*/
template<class Dummy>
const ULong ULong_constants_<Dummy>::ONE = ULong(1);

typedef ULong_constants_<void> ULong_constants;

/*!
 @brief Unsigned BigInteger.
 */
class ULong : public ULong_constants
{
    //friend std::ostream& operator<<(std::ostream& os, const ULong& l);
    //friend std::istream& operator>>(std::istream& is, ULong& l);

public:
    static ULong random(size_t n);
    static ULong factorial(uddigit n);
    static ULong fact_odd(uddigit n);
    static ULong fact_even(uddigit n);

public:
    explicit ULong() : c_(0), l_(0), d_(nullptr) {}
    explicit ULong(uddigit u);
    ULong(const ULong& l);
    explicit ULong(const char *s, int base);
    ~ULong() { delete [] d_; }

    size_t capacity() const { return c_; }
    size_t length() const { return l_; }

    bool isOdd() const { return l_ > 0 && (d_[0] & 1) == 1; }
    bool isEven() const { return l_ < 1 || (d_[0] & 1) == 0; }

    std::string str(int base=10) const;
    void cstr(char *s, int base=10) const;
    void out(int base=10, bool br=true) const;
    void debug() const;

    ULong square() const;
    ULong sqrt() const;
    ULong pow(uddigit n) const;
    ULong karatsuba(const ULong& u) const;
    ULong divmod(const ULong& b, bool mod) const;
    ULong gcd(const ULong& b) const;
    ULong gcdBin(const ULong& b) const;
    uddigit bitLength() const;
    int cmp(const ULong& b) const;
    int cmp(uddigit b) const;

    inline bool operator!() const;
    inline ULong operator+() const;
    inline ULong& operator++();
    inline ULong operator++(int);
    inline ULong& operator--();
    inline ULong operator--(int);

    ULong operator+(const ULong& b) const;
    ULong operator-(const ULong& b) const;
    ULong operator*(const ULong& b) const;
    inline ULong operator/(const ULong& b) const;
    inline ULong operator%(const ULong& b) const;

    inline ULong operator+(uddigit b) const;
    inline ULong operator-(uddigit b) const;
    inline ULong operator*(uddigit b) const;
    inline ULong operator/(uddigit b) const;
    inline ULong operator%(uddigit b) const;
    ULong operator<<(uddigit n) const;
    ULong operator>>(uddigit n) const;

    ULong& operator=(const ULong& b);
    inline ULong& operator+=(const ULong& b);
    inline ULong& operator-=(const ULong& b);
    inline ULong& operator*=(const ULong& b);
    inline ULong& operator/=(const ULong& b);
    inline ULong& operator%=(const ULong& b);

    inline ULong& operator+=(uddigit b);
    inline ULong& operator-=(uddigit b);
    inline ULong& operator*=(uddigit b);
    inline ULong& operator/=(uddigit b);
    inline ULong& operator%=(uddigit b);
    inline ULong& operator<<=(uddigit n);
    inline ULong& operator>>=(uddigit n);

    inline bool operator==(const ULong& b) const;
    inline bool operator!=(const ULong& b) const;
    inline bool operator<(const ULong& b) const;
    inline bool operator>(const ULong& b) const;
    inline bool operator<=(const ULong& b) const;
    inline bool operator>=(const ULong& b) const;

    inline bool operator==(uddigit b) const;
    inline bool operator!=(uddigit b) const;
    inline bool operator<(uddigit b) const;
    inline bool operator>(uddigit b) const;
    inline bool operator<=(uddigit b) const;
    inline bool operator>=(uddigit b) const;

private:
    explicit ULong(const udigit *d, size_t size);

    void alloc(size_t length, bool zero);
    inline void norm();

    /*!
     capacity
     */
    size_t c_;

    /*!
     length
     */
    size_t l_;

    /*!
     digit blocks
     */
    udigit* d_;
};

}} // namespace ktn math

#include "ulong.inl"

#endif // KTN_MATH_ULONG_H_
