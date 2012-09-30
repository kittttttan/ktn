#pragma once

#include <ctime>
#include <string>

namespace ktn {

class Date {
    friend std::ostream& operator<<(std::ostream& os, const Date& d);
public:
    Date();
    Date(const Date& date);
    explicit Date(time_t time);
    explicit Date(int year, int month=1, int day=1,
        int hour=0, int min=0, int sec=0);
    ~Date();

    static const size_t dateFormatMaxLength = 32;
    static const char defaultDateFormat[dateFormatMaxLength];

    static const time_t MINUTE = 60;
    static const time_t HOUR = 60 * MINUTE;
    static const time_t DAY = 24 * HOUR;
    static const time_t WEEK = 7 * DAY;

    static Date parse(const char* str);

    time_t time() const { return time_; }
    struct tm* tm() { return &date_; }
    int year() const { return date_.tm_year; }
    int month() const { return date_.tm_mon; }
    int date() const { return date_.tm_mday; }
    int day() const { return date_.tm_wday; }
    int dayOfYear() const { return date_.tm_yday; }
    int hour() const { return date_.tm_hour; }
    int minute() const { return date_.tm_min; }
    int second() const { return date_.tm_sec; }
    char* dateFormat() const { return dateformat_; }

    void setYear(int year) { date_.tm_year = year - 1900; time_ = mktime(&date_); }
    void setMonth(int month) { date_.tm_mon = month - 1; time_ = mktime(&date_); }
    void setDate(int day) { date_.tm_mday = day; time_ = mktime(&date_); }
    void setHour(int hour) { date_.tm_hour = hour; time_ = mktime(&date_); }
    void setMinute(int min) { date_.tm_min = min; time_ = mktime(&date_); }
    void setSecond(int sec) { date_.tm_sec = sec; time_ = mktime(&date_); }

    void addYear(int year) { date_.tm_year += year; time_ = mktime(&date_); }
    void addMonth(int month) { date_.tm_mon += month; time_ = mktime(&date_); }
    void addDate(int day) { date_.tm_mday += day; time_ = mktime(&date_); }
    void addHour(int hour) { date_.tm_hour += hour; time_ = mktime(&date_); }
    void addMinute(int min) { date_.tm_min += min; time_ = mktime(&date_); }
    void addSecond(int sec) { date_.tm_sec += sec; time_ = mktime(&date_); }

    void setDateFormat(char* format) { dateformat_ = format; }

    std::string str() const;
    Date& operator=(const Date& d);
    Date& operator+=(time_t time);
    Date& operator-=(time_t time);

    Date operator+(time_t time) const;
    Date operator-(time_t time) const;

    bool operator==(const Date& d) const;
    bool operator!=(const Date& d) const;
    bool operator>(const Date& d) const;
    bool operator<(const Date& d) const;
    bool operator>=(const Date& d) const;
    bool operator<=(const Date& d) const;
private:
    time_t time_;
    struct tm date_;
    char* dateformat_;
};

} // namespace ktn