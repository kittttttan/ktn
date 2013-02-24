#pragma once
#ifndef UTIL_H_
#define UTIL_H_

#define SAFE_DELETE(x)  \
    {                   \
        delete x;       \
        x = nullptr;    \
    }

#define SAFE_DELETE_ARRAY(x)    \
    {                           \
        delete [] x;            \
        x = nullptr;            \
    }

#endif // UTIL_H_