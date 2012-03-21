#ifndef KTN_STRING_H_
#define KTN_STRING_H_

/**
 * @file  string.h
 * @brief String
 */

#ifdef _MSC_VER
#include <tchar.h>
#else
#include "tchar.h"
#endif
#include <iostream>
#ifdef _DEBUG
#define TRACE(s)	{std::wcout<<s<<std::endl;}
#else
#define TRACE(s)
#endif

#ifdef _UNICODE
#define _tostream wostream
#define _tistream wistream
#else
#define _tostream ostream
#define _tistream oistream
#endif

namespace ktn {

/**
 * @brief String
 */
class String {

friend std::_tostream& operator<<(std::_tostream& os, const String& s);
//friend std::_tistream& operator>>(std::_tistream &is, String& s);

public:
	String() : string_(nullptr), length_(0) {}
	explicit String(int n);
	explicit String(const TCHAR* s);
	String(const String& s);
	~String() { delete [] string_; }

	TCHAR* string() { return string_; }
	int length() { return length_; }

	void out() const;
	int indexOf(const TCHAR c, int from=0) const;
	int lastIndexOf(const TCHAR c, int from=0) const;
	String trim();
	String trimLeft();
	String trimRight();
	String toUpperCase();
	String toLowerCase();
	String substr(int from, int length);
	String slice(int from, int to=-1);

	TCHAR& operator[](int index);
	bool operator!() const;
	String operator-() const;

	String operator+(const String& s) const;
	String operator-(const TCHAR c) const;
	String operator*(int times) const;

	String& operator=(const String& s);
	String& operator+=(const String& b);
	String& operator-=(const TCHAR c);
	String& operator*=(int times);

	bool operator==(const String& s) const;
	bool operator!=(const String& s) const;

private:
	String(const TCHAR* s, int length);

	TCHAR* string_;
	int length_;
};

} // namespace ktn
#endif // KTN_STRING_H_