#ifndef _JOJ_MATH_TYPES_H
#define _JOJ_MATH_TYPES_H

#include "joj/core/defines.h"

#define J_PI 3.14159265358979323846f
#define JDEG2RAD (J_PI / 180.0f)

namespace joj
{    
    struct JOJ_API vec2
    {
        union
        {
            f32 elem[2];
            struct
            {
                union
                {
                    f32 x, s, u;
                };
                union
                {
                    f32 y, t, v;
                };
            };
        };
    };

    struct JOJ_API vec3
    {
        union
        {
            f32 elem[3];
            struct
            {
                union
                {
                    f32 x, r, s, u;
                };
                union
                {
                    f32 y, g, t, v;
                };
                union
                {
                    f32 z, b, p, w;
                };
            };
        };
    };

    struct JOJ_API vec4
    {
        union
        {
            f32 elem[4];
            struct
            {
                union
                {
                    f32 x, r, s;
                };
                union
                {
                    f32 y, g, t;
                };
                union
                {
                    f32 z, b, p;
                };
                union
                {
                    f32 w, a, q;
                };
            };
        };
    };

    struct JOJ_API mat4x4
    {
        union
        {
            f32 elem[4][4];
            f32 data[16];
        };
    };
}

#endif // _JOJ_MATH_TYPES_H