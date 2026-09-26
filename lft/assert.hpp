#ifndef _LFT_ASSERT_HPP
#define _LFT_ASSERT_HPP

#include "lib.hpp"

#define LFT_ASSERT(cond)  \
    do                    \
    {                     \
        if (!(cond))      \
            lft::abort(); \
    }                     \
    while (0);

#if LFT_MODE_DEBUG
#define LFT_ASSERT_DEBUG(cond) \
    do                         \
    {                          \
        if (!(cond))           \
            lft::abort();      \
    }                          \
    while (0);
#else
#define LFT_ASSERT_DEBUG(cond)
#endif

#endif // _LFT_ASSERT_HPP
