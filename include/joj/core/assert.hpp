#ifndef _JOJ_ASSERT_HPP
#define _JOJ_ASSERT_HPP

#include "joj/core/error/error_code.hpp"
#include "joj/core/lib.hpp"

#define JOJ_ASSERT(cond)  \
    do                    \
    {                     \
        if (!(cond))      \
            joj::abort(); \
    }                     \
    while (0);

#if JOJ_MODE_DEBUG
#define JOJ_ASSERT_DEBUG(cond) \
    do                         \
    {                          \
        if (!(cond))           \
            joj::abort();      \
    }                          \
    while (0);
#else
#define JOJ_ASSERT_DEBUG(cond)
#endif

#define JOJ_FAILED(err_code) ((err_code) != (joj::ErrorCode::OK))

#endif // _JOJ_ASSERT_HPP
