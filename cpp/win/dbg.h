#ifndef DBG_H_
#define DBG_H_

#ifndef __func__
#ifdef __FUNCTION__
#define __func__ __FUNCTION__
#else
#define __func__ ""
#endif
#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define _ASSERTE(expr)
#endif

#ifdef _DEBUG
#include <stdio.h>
#define TRACE(...)                                                      \
    {                                                                   \
        _tprintf(_T("%s[%d]%s "), _T(__FILE__), __LINE__, _T(__func__));\
        _tprintf(__VA_ARGS__);                                          \
        _tprintf(_T("\n"));                                             \
    }
#else
#define TRACE(...)
#endif

#endif // DBG_H_