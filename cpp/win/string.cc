#include "string.h"
#include "util.h"
#include "dbg.h"
#include <cstdio>
#include <clocale>
#include <algorithm>
#include <cassert>
#include <sstream>

namespace ktn {

String::String()
{

}

String::String(const char* s) :
    str_(s)
{

}

String::String(const String& s)
{
    if (this == &s) { return; }
    str_ = s.str_;
}

String::~String()
{

}

String& String::operator=(const String& s)
{
    if (this == &s) { return *this; }
    str_ = s.str_;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const String& s)
{
    return os << s.str_.c_str();
}

std::istream& operator>>(std::istream& is, String& s)
{
    std::string str;
    is >> str;
    s.str_ = str;
    return is;
}

char& String::operator[](int index)
{
    if (index < 0) {
        index += static_cast<int>(str_.size());
    }
    return str_[index];
}

/**
 * @return true if string is empty, otherwise false
 */
bool String::operator!() const
{
    return str_.empty();
}

/**
 * @return reversed string
 */
String String::operator-() const
{
    String res(*this);
    std::reverse(res.str_.begin(), res.str_.end());
    return res;
}

size_t String::indexOf(const char c, int from) const
{
    return str_.find(c);
}

size_t String::lastIndexOf(const char c, int from) const
{
    return str_.rfind(c);
}

String& String::toUpperCase()
{
    std::transform(str_.begin(), str_.end(), str_.begin(), ::toupper);

    return *this;
}

String& String::toLowerCase()
{
    std::transform(str_.begin(), str_.end(), str_.begin(), ::tolower);

    return *this;
}

String& String::substr(int from, int length)
{
    str_ = str_.substr(from, length);
    return *this;
}

String& String::slice(int from, int to)
{
    if (str_.empty()) return *this;

    const int size = static_cast<int>(str_.size());
    assert(size);
    from %= size;
    to %= size;
    if (from < 0) { from += size; }
    if (to < 0) { to += size; }

    const int length = to - from + 1;
    if (length <= 0) { str_ = ""; return *this; }

    return substr(from, length);
}

String& String::trimLeft(const char* delim)
{
    const size_t p1 = str_.find_first_not_of(delim);
    if (p1 == std::string::npos) {
        str_ = "";
        return *this;
    }
    str_ = str_.substr(p1);
    return *this;
}

String& String::trimRight(const char* delim)
{
    const size_t p2 = str_.find_last_not_of(delim);
    str_ = str_.substr(0, p2 + 1);
    return *this;
}

String& String::trim(const char* delim)
{
    const size_t p1 = str_.find_first_not_of(delim);
    if(p1 == std::string::npos){
        str_ = "";
        return *this;
    }
    const size_t p2 = str_.find_last_not_of(delim);
    str_ = str_.substr(p1, p2 - p1 + 1);
    return *this;
}

String String::operator+(const String& s) const
{
    return String((str_ + s.str_).c_str());
}

String& String::operator+=(const String& b)
{
    str_ += b.str_;
    return *this;
}

String& String::operator+=(const char* b)
{
    str_ += b;
    return *this;
}

/**
 * erase specific chars from string
 */
String& String::operator-=(const char c)
{
    str_.erase(
        std::remove_if(str_.begin(), str_.end(),
            [c](char ch) -> bool { return ch == c; }),
        str_.end()
    );
    return *this;
}

/**
 * @return repeated string. negative argument means reversed.
 */
String& String::operator*=(int times)
{
    if (!times) {
        str_ = "";
        return *this;
    }

    std::string str(str_);
    if (times < 0) {
        for (int i = 1; i < -times; ++i) {
            str_ += str;
        }
        std::reverse(str_.begin(), str_.end());
    } else {
        for (int i = 1; i < times; ++i) {
            str_ += str;
        }
    }

    return *this;
}

bool String::operator==(const String& s) const
{
    if (this == &s) { return true; }
    return str_.compare(s.str_) == 0;
}

bool String::operator!=(const String& s) const
{
    if (this == &s) { return false; }
    return str_.compare(s.str_) != 0;
}

} // namespace ktn
