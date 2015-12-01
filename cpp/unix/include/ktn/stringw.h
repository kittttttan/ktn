#pragma once
#ifndef KTN_STRINGW_H_
#define KTN_STRINGW_H_

/**
 * @file  stringw.h
 * @brief StringW
 */
#include <iostream>

namespace ktn {

/**
 * @brief String
 */
class StringW {

    friend std::wostream& operator<<(std::wostream& os, const StringW& s);
    //friend std::wistream& operator>>(std::wistream &is, String& s);

public:
    StringW();
    explicit StringW(int n);
    explicit StringW(const wchar_t* s);
    StringW(const StringW& s);
    ~StringW();

    wchar_t* string() const { return string_; }
    int length() const { return length_; }

    void out() const;
    int indexOf(const wchar_t c, int from=0) const;
    int lastIndexOf(const wchar_t c, int from=0) const;
    StringW trim();
    StringW trimLeft();
    StringW trimRight();
    StringW toUpperCase();
    StringW toLowerCase();
    StringW substr(int from, int length);
    StringW slice(int from, int to=-1);

    wchar_t& operator[](int index);
    bool operator!() const;
    StringW operator-() const;

    StringW operator+(const StringW& s) const;
    StringW operator-(const wchar_t c) const;
    StringW operator*(int times) const;

    StringW& operator=(const StringW& s);
    StringW& operator+=(const StringW& b);
    StringW& operator-=(const wchar_t c);
    StringW& operator*=(int times);

    bool operator==(const StringW& s) const;
    bool operator!=(const StringW& s) const;

private:
    StringW(const wchar_t* s, int length);

    wchar_t* string_;
    int length_;
};

} // namespace ktn
#endif // KTN_STRINGW_H_