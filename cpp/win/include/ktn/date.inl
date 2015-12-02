#pragma once

/*!
 @file  date.inl
 @brief inline functions for Date
 */
//#include "ktn/date.h"

#include <stdlib.h>
#include <string.h>
#include <exception>
#include <sstream>

namespace ktn {

inline std::string Date::str() const
{
    return df_.format(*this);
}

inline std::wstring Date::wstr() const
{
    return df_.wformat(*this);
}

inline Date Date::operator+(time_t time) const
{
    return Date(time_ + time);
}

inline Date Date::operator-(time_t time) const
{
    return Date(time_ - time);
}

inline Date& Date::operator=(const Date& d)
{
    if (this == &d) { return *this; }

    time_ = d.time_;
    errno_t err = localtime_s(&date_, &time_);
    if (err) {
        throw std::runtime_error("Failed localtime_s");
    }

    return *this;
}

inline Date& Date::operator+=(time_t time)
{
    time_ += time;
    return *this;
}

inline Date& Date::operator-=(time_t time)
{
    time_ -= time;
    return *this;
}

inline bool Date::operator==(const Date& d) const
{
    return time_ == d.time_;
}

inline bool Date::operator!=(const Date& d) const
{
    return time_ != d.time_;
}

inline bool Date::operator<(const Date& d) const
{
    return time_ < d.time_;
}

inline bool Date::operator>(const Date& d) const
{
    return time_ > d.time_;
}

inline bool Date::operator<=(const Date& d) const
{
    return time_ <= d.time_;
}

inline bool Date::operator>=(const Date& d) const
{
    return time_ >= d.time_;
}

inline Date::Date()
{
	::time(&time_);
	errno_t err = localtime_s(&date_, &time_);
	if (err) {
		throw std::runtime_error("Failed localtime_s");
	}
}

inline Date::Date(int year, int month, int day,
	int hour, int min, int sec)
{
	date_.tm_year = year - 1900;
	date_.tm_mon = month - 1;
	date_.tm_mday = day;
	date_.tm_hour = hour;
	date_.tm_min = min;
	date_.tm_sec = sec;

	time_ = mktime(&date_);
}

inline Date& Date::time(time_t t)
{
	time_ = t;
	errno_t err = localtime_s(&date_, &time_);
	if (err) {
		throw std::runtime_error("Failed localtime_s");
	}

	return *this;
}

inline std::ostream& operator<<(std::ostream& os, const Date& d)
{
	return os << d.str();
}

inline std::wostream& operator<<(std::wostream& os, const Date& d)
{
	return os << d.wstr();
}

inline std::istream& operator>>(std::istream &is, Date& d)
{
	std::string str;
	is >> str;
	d = Date::parse(str.c_str());
	return is;
}

inline Date Date::parse(const char* str)
{
	std::istringstream is(str);
	int year = 1900, month = 1, day = 1,
		hour = 0, min = 0, sec = 0;
	char c;

	is >> year >> c >> month >> c >> day
		>> hour >> c >> min >> sec;

	Date d;
	struct tm& date = d.date_;
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;
	date.tm_hour = hour;
	date.tm_min = min;
	date.tm_sec = sec;
	d.time_ = mktime(&date);

	return d;
}

} // namespace ktn
