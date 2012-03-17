#ifndef JS_STRING_H_
#define JS_STRING_H_

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

namespace ktn {
class String {
public:
	String() : string_(nullptr), length_(0) {}
	explicit String(int n);
	explicit String(const TCHAR* s);
	String(const String& s);
	~String() { delete [] string_; }

	TCHAR* string() { return string_; }
	int length() { return length_; }

	void out();
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

private:
	String(const TCHAR* s, int length);

	TCHAR* string_;
	int length_;
};

} // namespace ktn
#endif // JS_STRING_H_