#include "long.h"
#include "../dbg.h"

#include <cstdio>
#include <sstream>

namespace ktn { namespace math {

const Long Long::ZERO("", 10);
const Long Long::ONE(1);

Long::Long(const Long& l)
{
    if (this == &l) { return; }

    s_ = l.s_;
    u_ = l.u_;
}

Long::Long(ddigit a)
{
    if (a < 0) {
        s_ = false;
        u_ = ULong(-a);
    } else {
        s_ = true;
        u_ = ULong(a);
    }
}

Long::Long(const char *s, int base)
{
    if (*s == '-') {
        s_ = false;
        ++s;
    } else {
        s_ = true;
    }
    u_ = ULong(s, base);
}

void Long::debug() const
{
    if (!s_) {
        putchar('-');
    }
    u_.debug();
}

std::ostream& operator<<(std::ostream& os, const Long& l)
{
    return os << l.str(os.hex ? 16 : 10);
}

std::istream& operator>>(std::istream& is, Long& l)
{
    std::string str;
    is >> str;
    l = Long(str.c_str(), is.hex ? 16 : 10);
    return is;
}

std::string Long::str(int base) const
{
    std::string s = u_.str(base);
    if (!s_) { s = "-" + s; }

    return s;
}

void Long::cstr(char *s, int base) const
{
    if (!s_) {
        *s++ = '-';
    }
    u_.cstr(s, base);
}

void Long::out(int base) const
{
    if (!s_) { putchar('-'); }
    u_.out(base);
}

}} // namespace ktn math