#ifndef _JOJ_MATH_TYPES_H
#define _JOJ_MATH_TYPES_H

#include "joj/core/defines.h"

namespace joj
{
    /**
     * @brief A structure that represents a 2D vector.
     */
    struct JOJ_API vec2
    {
        /** @brief The elements of the vector. */
        union
        {
            /** @brief The elements of the vector in array form. */
            f32 elem[2];
            struct
            {
                /** @brief The first component of the vector. */
                union
                {
                    f32 x, s, u;
                };

                /** @brief The second component of the vector. */
                union
                {
                    f32 y, t, v;
                };
            };
        };
    };

    /**
     * @brief A structure that represents a 3D vector.
     */
    struct JOJ_API vec3
    {
        /** @brief The elements of the vector. */
        union
        {
            /** @brief The elements of the vector in array form. */
            f32 elem[3];
            struct
            {
                /** @brief The first component of the vector. */
                union
                {
                    f32 x, r, s, u;
                };

                /** @brief The second component of the vector. */
                union
                {
                    f32 y, g, t, v;
                };

                /** @brief The third component of the vector. */
                union
                {
                    f32 z, b, p, w;
                };
            };
        };
    };

    /**
     * @brief A structure that represents a 4D vector.
     */
    struct JOJ_API vec4
    {
        /** @brief The elements of the vector. */
        union
        {
            /** @brief The elements of the vector in array form. */
            f32 elem[4];
            struct
            {
                /** @brief The first component of the vector. */
                union
                {
                    f32 x, r, s;
                };

                /** @brief The second component of the vector. */
                union
                {
                    f32 y, g, t;
                };

                /** @brief The third component of the vector. */
                union
                {
                    f32 z, b, p;
                };

                /** @brief The fourth component of the vector. */
                union
                {
                    f32 w, a, q;
                };
            };
        };
    };

    /**
     * @brief A structure that represents a 2D matrix.
     */
    struct JOJ_API mat4x4
    {
        /** @brief The elements of the matrix. */
        union
        {
            /** @brief The elements of the matrix in 'matrix' form. */
            f32 elem[4][4];

            /** @brief The elements of the matrix in array form. */
            f32 data[16];
        };
    };
}

#endif // _JOJ_MATH_TYPES_H