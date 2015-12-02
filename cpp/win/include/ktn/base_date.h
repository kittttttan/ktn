#pragma once

namespace ktn {

struct BaseDate
{
    virtual ~BaseDate() {}
    virtual const struct tm* tm() const = 0;
};

} // namespace ktn
