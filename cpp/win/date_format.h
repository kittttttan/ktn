#pragma once
#include <string>

namespace ktn {

class Date;

class DateFormat {
public:
    DateFormat() {}
    virtual ~DateFormat() {}

    virtual std::string format(const Date& date) const;
    virtual std::wstring wformat(const Date& date) const;
};

} // namespace ktn