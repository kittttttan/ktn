#include "fraction.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>

namespace ktn { namespace math {

const Fraction Fraction::ZERO(0);
const Fraction Fraction::ONE(1);

Fraction::Fraction(ddigit a) :
    d_(1)
{
    if (a < 0) {
        s_ = false;
        n_ = ULong(-a);
    } else {
        s_ = true;
        n_ = ULong(a);
    }
}

/*!
 @throw std::logic_error
 */
Fraction::Fraction(const ULong& a, const ULong& b, bool s) :
    s_(s)
{
    n_ = a;
    d_ = b;
    if (!d_) {
        throw std::logic_error("ZeroDivision");
    }
    cancel();
}

Fraction::Fraction(const char *s, int radix)
{
    // TODO:
    throw "Not implementation";
}

Fraction::Fraction(const Fraction& f)
{
    if (this == &f) { return; }
    s_ = f.s_;
    n_ = f.n_;
    d_ = f.d_;
}

inline void Fraction::cancel()
{
    ULong g(n_.gcdBin(d_));
    n_ /= g;
    d_ /= g;
}

/*!
 @throw std::logic_error
 */
Fraction::Fraction(ddigit a, ddigit b)
{
    if (a < 0) {
        s_ = false;
        n_ = ULong(-a);
    } else {
        s_ = true;
        n_ = ULong(a);
    }
    if (b == 0) {
        throw std::logic_error("ZeroDivision");
    } else if (b < 0) {
        s_ = !s_;
        d_ = ULong(-b);
    } else {
        d_ = ULong(b);
    }

    cancel();
}

std::string Fraction::str(int radix) const
{
    std::string s("");
    if (!s_) { s = "-"; }
    s += n_.str(radix);
    s += "/";
    s += d_.str(radix);

    return s;
}

std::ostream& operator<<(std::ostream& os, const Fraction& f)
{
    return os << f.str(10);
}

std::istream& operator>>(std::istream& is, Fraction& f)
{
    std::string str;
    is >> str;
    f = Fraction(str.c_str(), 10);
    return is;
}

void Fraction::out(int radix, bool br) const
{
    if (!s_) { putchar('-'); }
    n_.out(radix, false);
    putchar('/');
    d_.out(radix, false);
    if (br) { puts(""); }
}

}} // namespace ktn math
