#pragma once

#include "date_format.h"

#include <time.h>
#include <string>

namespace ktn {

class Date
{
    friend std::ostream& operator<<(std::ostream& os, const Date& d);
    friend std::wostream& operator<<(std::wostream& os, const Date& d);
    friend std::istream& operator>>(std::istream &is, Date& d);

public:
    static const time_t MINUTE = 60;
    static const time_t HOUR = 60 * MINUTE;
    static const time_t DAY = 24 * HOUR;
    static const time_t WEEK = 7 * DAY;

    static Date parse(const char* str);

public:
    Date();
    Date(const Date& date)
    {
        time(date.time_);
    }

    explicit Date(time_t t)
    {
        time(t);
    }

    explicit Date(int year, int month=1, int day=1,
        int hour=0, int min=0, int sec=0);
    ~Date() {}

    time_t time() const { return time_; }
    const struct tm* tm() const { return &date_; }
    int year() const { return date_.tm_year; }
    int month() const { return date_.tm_mon; }
    int date() const { return date_.tm_mday; }
    int day() const { return date_.tm_wday; }
    int dayOfYear() const { return date_.tm_yday; }
    int hour() const { return date_.tm_hour; }
    int minute() const { return date_.tm_min; }
    int second() const { return date_.tm_sec; }

    Date& time(time_t t);
    Date& year(int year) { date_.tm_year = year - 1900; time_ = mktime(&date_); return *this; }
    Date& month(int month) { date_.tm_mon = month - 1; time_ = mktime(&date_); return *this; }
    Date& date(int day) { date_.tm_mday = day; time_ = mktime(&date_); return *this; }
    Date& hour(int hour) { date_.tm_hour = hour; time_ = mktime(&date_); return *this; }
    Date& minute(int min) { date_.tm_min = min; time_ = mktime(&date_); return *this; }
    Date& second(int sec) { date_.tm_sec = sec; time_ = mktime(&date_); return *this; }

    Date& addYear(int year) { date_.tm_year += year; time_ = mktime(&date_); return *this; }
    Date& addMonth(int month) { date_.tm_mon += month; time_ = mktime(&date_); return *this; }
    Date& addDate(int day) { date_.tm_mday += day; time_ = mktime(&date_); return *this; }
    Date& addHour(int hour) { date_.tm_hour += hour; time_ = mktime(&date_); return *this; }
    Date& addMinute(int min) { date_.tm_min += min; time_ = mktime(&date_); return *this; }
    Date& addSecond(int sec) { date_.tm_sec += sec; time_ = mktime(&date_); return *this; }

    inline std::string str() const;
    inline std::wstring wstr() const;
    inline Date& operator=(const Date& d);
    inline Date& operator+=(time_t time);
    inline Date& operator-=(time_t time);

    inline Date operator+(time_t time) const;
    inline Date operator-(time_t time) const;

    inline bool operator==(const Date& d) const;
    inline bool operator!=(const Date& d) const;
    inline bool operator>(const Date& d) const;
    inline bool operator<(const Date& d) const;
    inline bool operator>=(const Date& d) const;
    inline bool operator<=(const Date& d) const;
private:
    time_t time_;
    struct tm date_;
    DateFormat df_;
};

} // namespace ktn

#include "date.inl"
