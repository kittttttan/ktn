#ifndef KTN_STRING_H_
#define KTN_STRING_H_

/**
 * @file  string.h
 * @brief String
 */
#include <iostream>

namespace ktn {

/**
 * @brief String
 */
class String {

    friend std::ostream& operator<<(std::ostream& os, const String& s);
    //friend std::istream& operator>>(std::istream &is, String& s);

public:
    String();
    explicit String(int n);
    explicit String(const char* s);
    String(const String& s);
    ~String();

    char* string() const { return string_; }
    int length() const { return length_; }

    void out() const;
    int indexOf(const char c, int from=0) const;
    int lastIndexOf(const char c, int from=0) const;
    String trim();
    String trimLeft();
    String trimRight();
    String toUpperCase();
    String toLowerCase();
    String substr(int from, int length);
    String slice(int from, int to=-1);

    char& operator[](int index);
    bool operator!() const;
    String operator-() const;

    String operator+(const String& s) const;
    String operator-(const char c) const;
    String operator*(int times) const;

    String& operator=(const String& s);
    String& operator+=(const String& b);
    String& operator-=(const char c);
    String& operator*=(int times);

    bool operator==(const String& s) const;
    bool operator!=(const String& s) const;

private:
    String(const char* s, int length);

    char* string_;
    int length_;
};

} // namespace ktn
#endif // KTN_STRING_H_