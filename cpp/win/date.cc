#include "date.h"
#include <sstream>
#include <cstring>

namespace ktn {

const char Date::defaultDateFormat[dateFormatMaxLength]
    = "%Y-%m-%d %H:%M:%S";

Date::Date() :
    dateformat_(nullptr)
{
    ::time(&time_);
    errno_t err = localtime_s(&date_, &time_);
    if (err) {
        // TODO:
    }
}

Date::Date(const Date& date) {
    time_ = date.time_;
    errno_t err = localtime_s(&date_, &time_);
    dateformat_ = date.dateformat_;
}

Date::Date(time_t time) :
    dateformat_(nullptr)
{
    time_ = time;
    errno_t err = localtime_s(&date_, &time_);
}

Date::Date(int year, int month, int day,
        int hour, int min, int sec) :
    dateformat_(nullptr)
{
    date_.tm_year = year - 1900;
    date_.tm_mon = month - 1;
    date_.tm_mday = day;
    date_.tm_hour = hour;
    date_.tm_min = min;
    date_.tm_sec = sec;

    time_ = mktime(&date_);
}

Date::~Date() {

}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    return os << d.str();
}

Date Date::parse(const char* str) {
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

std::string Date::str() const {
    char res[dateFormatMaxLength];
    strftime(res, dateFormatMaxLength - 1,
        (dateformat_ ? dateformat_ : defaultDateFormat), &date_);
    
    return std::string(res);
}

Date Date::operator+(time_t time) const {
    return Date(time_ + time);
}

Date Date::operator-(time_t time) const {
    return Date(time_ - time);
}

Date& Date::operator=(const Date& d) {
    if (this == &d) { return *this; }

    time_ = d.time_;
    errno_t err = localtime_s(&date_, &time_);
    dateformat_ = d.dateformat_;

    return *this;
}

Date& Date::operator+=(time_t time) {
    *this = *this + time;
    return *this;
}

Date& Date::operator-=(time_t time) {
    *this = *this - time;
    return *this;
}

bool Date::operator==(const Date& d) const {
    return time_ == d.time_;
}

bool Date::operator!=(const Date& d) const {
    return time_ != d.time_;
}

bool Date::operator<(const Date& d) const {
    return time_ < d.time_;
}

bool Date::operator>(const Date& d) const {
    return time_ > d.time_;
}

bool Date::operator<=(const Date& d) const {
    return time_ <= d.time_;
}

bool Date::operator>=(const Date& d) const {
    return time_ >= d.time_;
}

} // namespace ktn