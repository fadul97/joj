#ifndef _JOJ_PRIMITIVE_MODE_H
#define _JOJ_PRIMITIVE_MODE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class PrimitiveMode
    {
        POINTS = 0,
        LINES = 1,
        LINE_LOOP = 2,
        LINE_STRIP = 3,
        TRIANGLES = 4,
        TRIANGLE_STRIP = 5,
        TRIANGLE_FAN = 6,
        UNKNOWN = -1
    };

    JOJ_API const char* primitive_mode_to_str(const PrimitiveMode& mode);
}

#endif // _JOJ_PRIMITIVE_MODE_H