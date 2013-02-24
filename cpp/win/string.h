#pragma once
#ifndef KTN_STRING_H_
#define KTN_STRING_H_

#include <iostream>

namespace ktn {

/**
 * @brief Extended std::string
 */
class String
{
    friend std::ostream& operator<<(std::ostream& os, const String& s);
    friend std::istream& operator>>(std::istream &is, String& s);

public:
    String();
    explicit String(const char* s);
    explicit String(const std::string& s);
    String(const String& s);
    ~String();

    const std::string& str() const { return str_; }

    size_t indexOf(const char c, int from=0) const;
    size_t lastIndexOf(const char c, int from=0) const;
    String& trim(const char* delim=" \t\r\n");
    String& trimLeft(const char* delim=" \t\r\n");
    String& trimRight(const char* delim=" \t\r\n");
    String& toUpperCase();
    String& toLowerCase();
    String& substr(int from, int length);
    String& slice(int from, int to=-1);

    char& operator[](int index);
    bool operator!() const;
    String operator-() const;

    String operator+(const String& s) const;

    String& operator=(const String& s);
    String& operator+=(const String& b);
    String& operator+=(const char* b);
    String& operator-=(const char c);
    String& operator*=(int times);

    bool operator==(const String& s) const;
    bool operator!=(const String& s) const;

private:
    std::string str_;
};

} // namespace ktn
#endif // KTN_STRING_H_