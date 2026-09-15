#ifndef _JOJ_TYPEDEFS_H
#define _JOJ_TYPEDEFS_H

#if __cplusplus < 202002L
#error "C++20 is required"
#endif

#if defined(_WIN32)
#define PLATFORM_NAME "windows"
#define JOJ_PLATFORM_WINDOWS
#elif defined(_WIN64)
#define PLATFORM_NAME "windows"
#define JOJ_PLATFORM_WINDOWS
#elif defined(__linux__)
#define PLATFORM_NAME "linux"
#define JOJ_PLATFORM_LINUX
#else
#error "Operating System not supported"
#endif

#endif // _JOJ_TYPEDEFS_H
