#pragma once
#ifndef DBG_H_
#define DBG_H_

#ifndef __func__
#define __func__ ""
#endif

#ifdef _DEBUG
#define _ASSERTE(expr) if(!(expr))                 \
    {                                              \
        fprintf(stderr, "%s: %s %s[%d]",           \
            __func__, #expr, __FILE__, __LINE__);  \
        std::exit(EXIT_FAILURE);                   \
    }
#else
#define _ASSERTE(expr)  ((void)0)
#endif

#ifdef _DEBUG
#include <cstdio>
#define TRACE(...)                                        \
    {                                                     \
        printf("%s[%d]%s ", __FILE__, __LINE__, __func__);\
        printf(__VA_ARGS__);                              \
        printf("\n");                                     \
    }
#else
#define TRACE(...)  ((void)0)
#endif

#endif // DBG_H_