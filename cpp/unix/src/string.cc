/**
 * @file  string.cc
 * @brief String
 */
#include "ktn/dbg.h"
#include "ktn/string.h"
#include "ktn/util.h"

#include <locale.h>
#include <stdio.h>
#include <vector>


namespace ktn {

String::String() :
    string_(nullptr),
    length_(0)
{
}

String::String(const char* s) {
    int length = 0;
    while (*s != '\0') {
        ++length;
        ++s;
    }

    try {
        string_ = new char[length + 1];
    } catch (std::bad_alloc) {
        string_ = nullptr;
        length_ = 0;
        return;
    }

    length_ = length;
    s -= length;
    for (int i = 0; i < length + 1; ++i) {
        string_[i] = *s;
        ++s;
    }
}

String::String(const char* s, int length) {
    try {
        string_ = new char[length + 1];
    } catch (std::bad_alloc) {
        string_ = nullptr;
        length_ = 0;
        return;
    }

    length_ = length;
    for (int i = 0; i < length + 1; ++i) {
        string_[i] = *s;
        ++s;
    }
}

String::String(const String& s) {
    if (this == &s) { return; }

    try {
        string_ = new char[s.length_ + 1];
    } catch (std::bad_alloc) {
        length_ = 0;
        return;
    }

    length_ = s.length_;

    for (int i = 0; i < length_ + 1; ++i) {
        string_[i] = s.string_[i];
    }
}

String::~String() {
    SAFE_DELETE_ARRAY(string_);
}

String& String::operator=(const String& s) {
    if (this == &s) { return *this; }

    if (length_ < s.length_) {
        delete [] string_;
        try {
            string_ = new char[s.length_ + 1];
        } catch (std::bad_alloc) {
            string_ = nullptr;
            length_ = 0;
            return *this;
        }
        length_ = s.length_;
    }
    for (int i = 0; i < length_ + 1; ++i) {
        string_[i] = s.string_[i];
    }

    return *this;
}

void String::out() const {
    if (length_ == 0 || !string_) {
        puts("");
        return;
    }
    printf("%s\n", string_);
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.string_;
}

//std::istream& operator<<(std::istream& is, String& s) {
//    return is;
//}

char& String::operator[](int index) {
    if (index < 0) {
        index += length_;
    }

    _ASSERTE(0 <= index && index < length_);

    return *(string_ + index);
}

bool String::operator!() const {
    return length_ == 0 || !string_;
}

String String::operator-() const {
    char* temp = nullptr;
    try {
        temp = new char[length_ + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    int i = 0;
    for (; i < length_; ++i) {
        temp[i] = string_[length_ - 1 - i];
    }
    temp[i] = '\0';

    String res(temp, length_);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

int String::indexOf(const char c, int from) const {
    int index = -1;
    for (int i = from; i < length_; ++i) {
        if (string_[i] == c) {
            index = i;
            break;
        }
    }

    return index;
}

int String::lastIndexOf(const char c, int from) const {
    int index = -1;
    if (from < 0) {
        from = 0;
    } else if (from > length_ - 1) {
        from = length_ - 1;
    }
    for (int i = from; i; --i) {
        if (string_[i] == c) {
            index = i;
            break;
        }
    }

    return index;
}

String String::toUpperCase() {
    char* w = nullptr;
    try {
        w = new char[length_ + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length_ + 1; ++i) {
        w[i] = towupper(string_[i]);
    }

    String res(w, length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String String::toLowerCase() {
    char* w = nullptr;
    try {
        w = new char[length_ + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length_ + 1; ++i) {
        w[i] = towlower(string_[i]);
    }

    String res(w, length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String String::substr(int from, int length) {
    if (length < 1) {
        return String();
    } else if (length > length_) {
        return *this;
    }
    if (from < 0) { from = 0; }
    if (from + length > length_) {
        length = length_ - from;
    }

    char* w = nullptr;
    try {
        w = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[from + i];
    }
    w[length] = '\0';

    String res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String String::slice(int from, int to) {
    if (length_ < 1) { return String(); }
  
    from %= length_;
    to %= length_;
    if (from < 0) { from += length_; }
    if (to < 0) { to += length_; }
    int length = to - from + 1;
    if (length <= 0) { return String(); }

    char* w = nullptr;
    try {
        w = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[from + i];
    }
    w[length] = '\0';

    String res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String String::trimLeft() {
    int i = 0;
    for (; i < length_; ++i) {
        if (iswspace(string_[i]) == 0) {
            break;
        }
    }
    int length = length_ - i;
    char* temp = nullptr;
    try {
        temp = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int j = 0; i < length_ + 1; ++i, ++j) {
        temp[j] = string_[i];
    }

    String res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

String String::trimRight() {
    if (length_ < 1) { return String(); }

    int i = length_ - 1;
    for (; i; --i) {
        if (iswspace(string_[i]) == 0) {
            break;
        }
    }
    int length = i + 1;
    char* temp = nullptr;
    try {
        temp = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }
    int j = 0;
    for (i = 0; i < length; ++i, ++j) {
        temp[j] = string_[i];
    }
    temp[j] = '\0';

    String res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

String String::trim() {
    if (length_ < 1) { return String(); }

    int first = 0;
    for (; first < length_; ++first) {
        if (iswspace(string_[first]) == 0) {
            break;
        }
    }
    int last = length_ - 1;
    for (; last; --last) {
        if (iswspace(string_[last]) == 0) {
            break;
        }
    }

    int length = last - first + 1;
    char* temp = nullptr;
    try {
        temp = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    int j = 0;
    for (int i = first; i < last + 1; ++i, ++j) {
        temp[j] = string_[i];
    }
    temp[j] = '\0';

    String res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

String String::operator+(const String& s) const {
    char* w = nullptr;
    try {
        w = new char[length_ + s.length_ + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length_; ++i) {
        w[i] = string_[i];
    }
    for (int i = 0; i < s.length_ + 1; ++i) {
        w[i + length_] = s.string_[i];
    }

    String res(w, length_ + s.length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String String::operator-(const char c) const {
    char* temp = nullptr;
    try {
        temp = new char[length_ + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    int length = 0;
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != c) {
            temp[length] = string_[i];
            ++length;
        }
    }
    temp[length] = '\0';

    String res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

String String::operator*(int times) const {
    if (length_ < 1 || times == 0) {
        return String();
    } else if (times < 0) {
        times = -times;

        int length = length_ * times;
        char* w = nullptr;
        try {
            w = new char[length + 1];
        } catch (std::bad_alloc) {
            return String();
        }

        for (int i = 0; i < length; ++i) {
            w[i] = string_[length_ - 1 - i % length_];
        }
        w[length] = '\0';

        String res(w, length);
        SAFE_DELETE_ARRAY(w);

        return res;
    }

    int length = length_ * times;
    char* w = nullptr;
    try {
        w = new char[length + 1];
    } catch (std::bad_alloc) {
        return String();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[i % length_];
    }
    w[length] = '\0';

    String res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

String& String::operator+=(const String& b) {
    *this = *this + b;
    return *this;
}

String& String::operator-=(const char c) {
    *this = *this - c;
    return *this;
}

String& String::operator*=(int times) {
    *this = *this * times;
    return *this;
}

bool String::operator==(const String& s) const {
    if (this == &s) { return true; }
    if (length_ != s.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != s.string_[i]) { return false; }
    }
    return true;
}

bool String::operator!=(const String& s) const {
    if (this == &s) { return false; }
    if (length_ != s.length_) { return true; }
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != s.string_[i]) { return true; }
    }
    return false;
}

} // namespace ktn
