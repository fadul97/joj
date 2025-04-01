#ifndef _JOJ_INTERPOLATION_TYPE_H
#define _JOJ_INTERPOLATION_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class InterpolationType
    {
        LINEAR,
        STEP,
        CUBICSPLINE,
        UNKNOWN
    };

    JOJ_API const char* interpolation_type_to_string(const InterpolationType type);
}

#endif // _JOJ_INTERPOLATION_TYPE_H