﻿#pragma once

#include "ktn/dbg.h"
#include "ktn/util.h"

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <algorithm>
#include <sstream>

namespace ktn {

inline StringW& StringW::operator=(const StringW& s)
{
	if (this == &s) { return *this; }

	str_ = s.str_;

	return *this;
}

inline std::wostream& operator<<(std::wostream& os, const StringW& s)
{
	return os << s.str_.c_str();
}

inline std::wistream& operator>>(std::wistream& is, StringW& s)
{
	std::wstring str;
	is >> str;
	s.str_ = str;
	return is;
}

inline wchar_t& StringW::operator[](int index)
{
	if (index < 0) {
		index += static_cast<int>(str_.size());
	}

	return str_[index];
}

inline bool StringW::operator!() const
{
	return str_.empty();
}

inline StringW StringW::operator-() const
{
	StringW res(*this);
	std::reverse(res.str_.begin(), res.str_.end());
	return res;
}

inline size_t StringW::indexOf(const wchar_t c, int from) const
{
	return str_.find(c);
}

inline size_t StringW::lastIndexOf(const wchar_t c, int from) const
{
	return str_.rfind(c);
}

inline StringW& StringW::toUpperCase()
{
	std::transform(str_.begin(), str_.end(), str_.begin(), ::toupper);
	return *this;
}

inline StringW& StringW::toLowerCase()
{
	std::transform(str_.begin(), str_.end(), str_.begin(), ::tolower);
	return *this;
}

inline StringW& StringW::substr(int from, int length)
{
	str_ = str_.substr(from, length);
	return *this;
}

inline StringW& StringW::slice(int from, int to)
{
	if (str_.empty()) return *this;

	int size = static_cast<int>(str_.size());
	assert(size);
	from %= size;
	to %= size;
	if (from < 0) { from += size; }
	if (to < 0) { to += size; }

	int length = to - from + 1;
	if (length <= 0) { str_ = L""; return *this; }

	return substr(from, length);
}

inline StringW& StringW::trimLeft(const wchar_t* delim)
{
	const size_t p1 = str_.find_first_not_of(delim);
	if (p1 == std::string::npos) {
		str_ = L"";
		return *this;
	}
	str_ = str_.substr(p1);
	return *this;
}

inline StringW& StringW::trimRight(const wchar_t* delim)
{
	const size_t p2 = str_.find_last_not_of(delim);
	str_ = str_.substr(0, p2 + 1);
	return *this;
}

inline StringW& StringW::trim(const wchar_t* delim)
{
	const size_t p1 = str_.find_first_not_of(delim);
	if (p1 == std::string::npos) {
		str_ = L"";
		return *this;
	}
	const size_t p2 = str_.find_last_not_of(delim);
	str_ = str_.substr(p1, p2 - p1 + 1);
	return *this;
}

inline StringW StringW::operator+(const StringW& s) const
{
	std::wstring n(str_);
	n += s.str_;
	return StringW(n);
}

inline StringW& StringW::operator+=(const StringW& b)
{
	str_ += b.str_;
	return *this;
}

inline StringW& StringW::operator+=(const wchar_t* b)
{
	str_ += b;
	return *this;
}

inline StringW& StringW::operator-=(const wchar_t c)
{
	str_.erase(
		std::remove_if(str_.begin(), str_.end(),
			[c](wchar_t ch) -> bool { return ch == c; }),
		str_.end()
		);
	return *this;
}

inline StringW& StringW::operator*=(int times)
{
	if (!times) {
		str_ = L"";
		return *this;
	}

	std::wstring str(str_);
	if (times < 0) {
		for (int i = 1; i < -times; ++i) {
			str_ += str;
		}
		std::reverse(str_.begin(), str_.end());
	}
	else {
		for (int i = 1; i < times; ++i) {
			str_ += str;
		}
	}

	return *this;
}

inline bool StringW::operator==(const StringW& s) const
{
	if (this == &s) { return true; }
	return str_.compare(s.str_) == 0;
}

inline bool StringW::operator!=(const StringW& s) const
{
	if (this == &s) { return false; }
	return str_.compare(s.str_) != 0;
}

} // namespace ktn
