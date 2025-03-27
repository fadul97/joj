#ifndef _JOJ_KEYFRAME_H
#define _JOJ_KEYFRAME_H

#include "joj/core/defines.h"

#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"

namespace joj
{
    struct JOJ_API KeyFrame
    {
        f32 time;
        Vector3 translation;
        Vector3 scale;
        Vector4 rotation;
    };
}

#endif // _JOJ_KEYFRAME_H