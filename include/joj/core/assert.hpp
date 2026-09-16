#ifndef _JOJ_ASSERT_HPP
#define _JOJ_ASSERT_HPP

#include "joj/core/lib.hpp"

#define JOJ_ASSERT(cond)  \
    do                    \
    {                     \
        if (!(cond))      \
            joj::abort(); \
    }                     \
    while (0);

#endif // _JOJ_ASSERT_HPP
