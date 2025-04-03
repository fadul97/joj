#ifndef _JOJ_VECTOR2_H
#define _JOJ_VECTOR2_H

#include "joj/core/defines.h"

namespace joj
{
    class JOJ_API Vector2
    {
    public:
        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(const f32 x, const f32 y) : x(x), y(y) {}

        f32 x;
        f32 y;
    };
}

#endif // _JOJ_VECTOR2_H