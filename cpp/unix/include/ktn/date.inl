#pragma once

/*!
 @file  date.inl
 @brief inline functions for Date
 */

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
    date_ = *localtime(&time_);

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

} // namespace ktn
