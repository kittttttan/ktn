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
class StringW
{
    friend std::wostream& operator<<(std::wostream& os, const StringW& s);
    friend std::wistream& operator>>(std::wistream &is, StringW& s);

public:
    StringW();
    explicit StringW(const wchar_t* s);
    explicit StringW(const std::wstring& s);
    StringW(const StringW& s);
    ~StringW();

    const std::wstring& str() const { return str_; }

    size_t indexOf(const wchar_t c, int from=0) const;
    size_t lastIndexOf(const wchar_t c, int from=0) const;
    StringW& trim(const wchar_t* delim=L" \t\r\n");
    StringW& trimLeft(const wchar_t* delim=L" \t\r\n");
    StringW& trimRight(const wchar_t* delim=L" \t\r\n");
    StringW& toUpperCase();
    StringW& toLowerCase();
    StringW& substr(int from, int length);
    StringW& slice(int from, int to=-1);

    wchar_t& operator[](int index);
    bool operator!() const;
    StringW operator-() const;

    StringW operator+(const StringW& s) const;

    StringW& operator=(const StringW& s);
    StringW& operator+=(const StringW& b);
    StringW& operator+=(const wchar_t* w);
    StringW& operator-=(const wchar_t c);
    StringW& operator*=(int times);

    bool operator==(const StringW& s) const;
    bool operator!=(const StringW& s) const;

private:
    std::wstring str_;
};

} // namespace ktn
#endif // KTN_STRINGW_H_