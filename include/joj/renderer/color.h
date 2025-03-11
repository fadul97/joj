#ifndef _JOJ_COLOR_H
#define _JOJ_COLOR_H

#include "joj/core/defines.h"

namespace joj
{
    struct JOJ_API Color
    {
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
        Color(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}

        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
}

#endif // _JOJ_COLOR_H