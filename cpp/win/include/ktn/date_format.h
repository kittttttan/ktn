#pragma once
#include "ktn/base_date.h"
#include <string>

namespace ktn {

class DateFormat {
public:
    DateFormat() {}
    virtual ~DateFormat() {}

    virtual std::string format(const BaseDate& date) const;
    virtual std::wstring wformat(const BaseDate& date) const;
};

} // namespace ktn

#include "ktn/date_format.inl"
