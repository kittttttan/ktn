﻿#pragma once
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
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#include <crtdbg.h>
#endif

#include <tchar.h>
#ifdef _DEBUG
#include <stdio.h>
#define TRACE(...)                                        \
    {                                                     \
        printf("%s[%d]%s ", __FILE__, __LINE__, __func__);\
        printf(__VA_ARGS__);                              \
        printf("\n");                                     \
    }
#define TRACEW(...)                                                 \
    {                                                               \
        wprintf(L"%s[%d]%s ", _T(__FILE__), __LINE__, _T(__func__));\
        wprintf(__VA_ARGS__);                                       \
        wprintf(L"\n");                                             \
    }
#else
#define TRACE(...)  ((void)0)
#define TRACEW(...) ((void)0)
#endif

#endif // DBG_H_