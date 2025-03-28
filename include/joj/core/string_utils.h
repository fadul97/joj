#ifndef _JOJ_STRING_UTILS_H
#define _JOJ_STRING_UTILS_H

#include "joj/core/defines.h"

namespace joj
{
    namespace StringUtils
    {
        JOJ_API i32 format(char* buffer, const i32 size, const char* format, ...);
    }
}

#endif // _JOJ_STRING_UTILS_H
