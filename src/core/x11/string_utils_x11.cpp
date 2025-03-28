#include "joj/core/string_utils.h"

#include <cstdio>
#include <cstdarg>

i32 joj::StringUtils::format(char* buffer, const i32 size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const i32 result = vsnprintf(buffer, static_cast<size_t>(size), format, args);
    va_end(args);
    return result;
}
