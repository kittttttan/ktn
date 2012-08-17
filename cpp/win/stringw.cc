/**
 * @file  stringw.cc
 * @brief StringW
 */

#include <cstdio>
#include <clocale>

#include "stringw.h"
#include "util.h"
#include "dbg.h"

namespace ktn {

StringW::StringW() :
    string_(nullptr),
    length_(0)
{
}

StringW::StringW(const wchar_t* s) {
    int length = 0;
    while (*s != L'\0') {
        ++length;
        ++s;
    }

    try {
        string_ = new wchar_t[length + 1];
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

StringW::StringW(const wchar_t* s, int length) {
    try {
        string_ = new wchar_t[length + 1];
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

StringW::StringW(const StringW& s) {
    if (this == &s) { return; }

    try {
        string_ = new wchar_t[s.length_ + 1];
    } catch (std::bad_alloc) {
        length_ = 0;
        return;
    }

    length_ = s.length_;

    for (int i = 0; i < length_ + 1; ++i) {
        string_[i] = s.string_[i];
    }
}

StringW::~StringW() {
    SAFE_DELETE_ARRAY(string_);
}

StringW& StringW::operator=(const StringW& s) {
    if (this == &s) { return *this; }

    if (length_ < s.length_) {
        delete [] string_;
        try {
            string_ = new wchar_t[s.length_ + 1];
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

void StringW::out() const {
    if (length_ == 0 || string_ == nullptr) {
        puts("");
        return;
    }
    wprintf(L"%s\n", string_);
}

std::wostream& operator<<(std::wostream& os, const StringW& s) {
    return os << s.string_;
}

//std::wistream& operator<<(std::wistream& is, StringW& s) {
//    return is;
//}

wchar_t& StringW::operator[](int index) {
    if (index < 0) {
        index += length_;
    }

    _ASSERTE(0 <= index && index < length_);

    return *(string_ + index);
}

bool StringW::operator!() const {
    return length_ == 0 || !string_;
}

StringW StringW::operator-() const {
    wchar_t* temp = nullptr;
    try {
        temp = new wchar_t[length_ + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    int i = 0;
    for (; i < length_; ++i) {
        temp[i] = string_[length_ - 1 - i];
    }
    temp[i] = L'\0';

    StringW res(temp, length_);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

int StringW::indexOf(const wchar_t c, int from) const {
    int index = -1;
    for (int i = from; i < length_; ++i) {
        if (string_[i] == c) {
            index = i;
            break;
        }
    }

    return index;
}

int StringW::lastIndexOf(const wchar_t c, int from) const {
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

StringW StringW::toUpperCase() {
    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length_ + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length_ + 1; ++i) {
        w[i] = towupper(string_[i]);
    }

    StringW res(w, length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW StringW::toLowerCase() {
    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length_ + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length_ + 1; ++i) {
        w[i] = towlower(string_[i]);
    }

    StringW res(w, length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW StringW::substr(int from, int length) {
    if (length < 1) {
        return StringW();
    } else if (length > length_) {
        return *this;
    }
    if (from < 0) { from = 0; }
    if (from + length > length_) {
        length = length_ - from;
    }

    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[from + i];
    }
    w[length] = L'\0';

    StringW res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW StringW::slice(int from, int to) {
    from %= length_;
    to %= length_;
    if (from < 0) { from += length_; }
    if (to < 0) { to += length_; }
    int length = to - from + 1;
    if (length <= 0) { return StringW(); }

    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[from + i];
    }
    w[length] = L'\0';

    StringW res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW StringW::trimLeft() {
    int i = 0;
    for (; i < length_; ++i) {
        if (iswspace(string_[i]) == 0) {
            break;
        }
    }
    int length = length_ - i;
    wchar_t* temp = nullptr;
    try {
        temp = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int j = 0; i < length_ + 1; ++i, ++j) {
        temp[j] = string_[i];
    }

    StringW res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

StringW StringW::trimRight() {
    if (length_ == 0) { return StringW(); }

    int i = length_ - 1;
    for (; i; --i) {
        if (iswspace(string_[i]) == 0) {
            break;
        }
    }
    int length = i + 1;
    wchar_t* temp = nullptr;
    try {
        temp = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }
    int j = 0;
    for (i = 0; i < length; ++i, ++j) {
        temp[j] = string_[i];
    }
    temp[j] = L'\0';

    StringW res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

StringW StringW::trim() {
    if (length_ == 0) { return StringW(); }

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
    wchar_t* temp = nullptr;
    try {
        temp = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    int j = 0;
    for (int i = first; i < last + 1; ++i, ++j) {
        temp[j] = string_[i];
    }
    temp[j] = L'\0';

    StringW res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

StringW StringW::operator+(const StringW& s) const {
    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length_ + s.length_ + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length_; ++i) {
        w[i] = string_[i];
    }
    for (int i = 0; i < s.length_ + 1; ++i) {
        w[i + length_] = s.string_[i];
    }

    StringW res(w, length_ + s.length_);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW StringW::operator-(const wchar_t c) const {
    wchar_t* temp = nullptr;
    try {
        temp = new wchar_t[length_ + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    int length = 0;
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != c) {
            temp[length] = string_[i];
            ++length;
        }
    }
    temp[length] = L'\0';

    StringW res(temp, length);
    SAFE_DELETE_ARRAY(temp);

    return res;
}

StringW StringW::operator*(int times) const {
    if (times == 0) {
        return StringW();
    } else if (times < 0) {
        times = -times;

        int length = length_ * times;
        wchar_t* w = nullptr;
        try {
            w = new wchar_t[length + 1];
        } catch (std::bad_alloc) {
            return StringW();
        }

        for (int i = 0; i < length; ++i) {
            w[i] = string_[length_ - 1 - i % length_];
        }
        w[length] = L'\0';

        StringW res(w, length);
        SAFE_DELETE_ARRAY(w);

        return res;
    }

    int length = length_ * times;
    wchar_t* w = nullptr;
    try {
        w = new wchar_t[length + 1];
    } catch (std::bad_alloc) {
        return StringW();
    }

    for (int i = 0; i < length; ++i) {
        w[i] = string_[i % length_];
    }
    w[length] = L'\0';

    StringW res(w, length);
    SAFE_DELETE_ARRAY(w);

    return res;
}

StringW& StringW::operator+=(const StringW& b) {
    *this = *this + b;
    return *this;
}

StringW& StringW::operator-=(const wchar_t c) {
    *this = *this - c;
    return *this;
}

StringW& StringW::operator*=(int times) {
    *this = *this * times;
    return *this;
}

bool StringW::operator==(const StringW& s) const {
    if (this == &s) { return true; }
    if (length_ != s.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != s.string_[i]) { return false; }
    }
    return true;
}

bool StringW::operator!=(const StringW& s) const {
    if (this == &s) { return false; }
    if (length_ != s.length_) { return true; }
    for (int i = 0; i < length_; ++i) {
        if (string_[i] != s.string_[i]) { return true; }
    }
    return false;
}

} // namespace ktn
