#ifndef _JOJ_JCMATH_H
#define _JOJ_JCMATH_H

#include "joj/core/defines.h"

#include "math_types.h"

namespace joj
{
    // -------------------------------------------------------------
    // Constants
    // -------------------------------------------------------------

    /** @brief The value of pi. */
    constexpr f32 J_PI = 3.14159265358979323846f;

    /** @brief The value of 2 * pi. */
    constexpr f32 J_2PI = 6.28318530717958647692f;

    /** @brief The value of pi / 180. */
    constexpr f32 J_DEG2RAD = (J_PI / 180.0f);

    /** @brief The value of 180 / pi. */
    constexpr f32 J_RAD2DEG = (180.0f / J_PI);

    /**
     * @brief Convert degrees to radians.
     * 
     * @param degrees The angle in degrees.
     * @return f32 The angle in radians.
     */
    inline f32 deg2rad(const f32 degrees)
    {
        return degrees * (J_PI / 180.0f);
    }

    /**
     * @brief Convert radians to degrees.
     * 
     * @param radians The angle in radians.
     * @return f32 The angle in degrees.
     */
    f32 rad2deg(const f32 radians)
    {
        return radians * (180.0f / J_PI);
    }

    // -------------------------------------------------------------
    // vec2 operations
    // -------------------------------------------------------------

    /**
     * @brief Create a vec2 object.
     * 
     * @param x The first component of the vector.
     * @param y The second component of the vector.
     * @return vec2 The created vec2 object.
     */
    inline vec2 vec2_create(const f32 x, const f32 y)
    {
        return vec2 { x, y };
    }

    /**
     * @brief Create a vec2 object with zero values.
     * 
     * @return vec2 The created vec2 object.
     */
    inline vec2 vec2_zero()
    {
        return vec2 { 0.0f, 0.0f };
    }

    /**
     * @brief Create a vec2 object with one values.
     * 
     * @return vec2 The created vec2 object.
     */
    inline vec2 vec2_one()
    {
        return vec2 { 1.0f, 1.0f };
    }

    /**
     * @brief Add two vec2 objects.
     * 
     * @param a The first vec2 object.
     * @param b The second vec2 object.
     * @return vec2 The result of the addition.
     */
    inline vec2 vec2_add(const vec2& a, const vec2& b)
    {
        return vec2 { a.x + b.x, a.y + b.y };
    }

    /**
     * @brief Subtract two vec2 objects.
     * 
     * @param a The first vec2 object.
     * @param b The second vec2 object.
     * @return vec2 The result of the subtraction.
     */
    inline vec2 vec2_sub(const vec2& a, const vec2& b)
    {
        return vec2 { a.x - b.x, a.y - b.y };
    }

    /**
     * @brief Multiply two vec2 objects.
     * 
     * @param a The first vec2 object.
     * @param b The second vec2 object.
     * @return vec2 The result of the multiplication.
     */
    inline vec2 vec2_mul(const vec2& a, const vec2& b)
    {
        return vec2 { a.x * b.x, a.y * b.y };
    }

    /**
     * @brief Divide two vec2 objects.
     * 
     * @param a The first vec2 object.
     * @param b The second vec2 object.
     * @return vec2 The result of the division.
     */
    inline vec2 vec2_div(const vec2& a, const vec2& b)
    {
        return vec2 { a.x / b.x, a.y / b.y };
    }

    /**
     * @brief Scale a vec2 object by a scalar value.
     * 
     * @param a The vec2 object.
     * @param scale The scalar value.
     * @return vec2 The scaled vec2 object.
     */
    inline vec2 vec2_scale(const vec2& a, const f32 scale)
    {
        return vec2 { a.x * scale, a.y * scale };
    }

    // -------------------------------------------------------------
    // vec3 operations
    // -------------------------------------------------------------

    /**
     * @brief Create a vec3 object.
     * 
     * @param x The first component of the vector.
     * @param y The second component of the vector.
     * @param z The third component of the vector.
     * @return vec3 The created vec3 object.
     */
    inline vec3 vec3_create(const f32 x, const f32 y, const f32 z)
    {
        return vec3 { x, y, z };
    }

    /**
     * @brief Create a vec3 object with zero values.
     * 
     * @return vec3 The created vec3 object.
     */
    inline vec3 vec3_zero()
    {
        return vec3 { 0.0f, 0.0f, 0.0f };
    }

    /**
     * @brief Create a vec3 object with one values.
     * 
     * @return vec3 The created vec3 object.
     */
    inline vec3 vec3_one()
    {
        return vec3 { 1.0f, 1.0f, 1.0f };
    }

    /**
     * @brief Add two vec3 objects.
     * 
     * @param a The first vec3 object.
     * @param b The second vec3 object.
     * @return vec3 The result of the addition.
     */
    inline vec3 vec3_add(const vec3& a, const vec3& b)
    {
        return vec3 { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    /**
     * @brief Subtract two vec3 objects.
     * 
     * @param a The first vec3 object.
     * @param b The second vec3 object.
     * @return vec3 The result of the subtraction.
     */
    inline vec3 vec3_sub(const vec3& a, const vec3& b)
    {
        return vec3 { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    /**
     * @brief Multiply two vec3 objects.
     * 
     * @param a The first vec3 object.
     * @param b The second vec3 object.
     * @return vec3 The result of the multiplication.
     */
    inline vec3 vec3_mul(const vec3& a, const vec3& b)
    {
        return vec3 { a.x * b.x, a.y * b.y, a.z * b.z };
    }

    /**
     * @brief Divide two vec3 objects.
     * 
     * @param a The first vec3 object.
     * @param b The second vec3 object.
     * @return vec3 The result of the division.
     */
    inline vec3 vec3_div(const vec3& a, const vec3& b)
    {
        return vec3 { a.x / b.x, a.y / b.y, a.z / b.z };
    }

    /**
     * @brief Scale a vec3 object by a scalar value.
     * 
     * @param a The vec3 object.
     * @param scale The scalar value.
     * @return vec3 The scaled vec3 object.
     */
    inline vec3 vec3_scale(const vec3& a, const f32 scale)
    {
        return vec3 { a.x * scale, a.y * scale, a.z * scale };
    }

    // -------------------------------------------------------------
    // vec4 operations
    // -------------------------------------------------------------

    /**
     * @brief Create a vec4 object.
     * 
     * @param x The first component of the vector.
     * @param y The second component of the vector.
     * @param z The third component of the vector.
     * @param w The fourth component of the vector.
     * @return vec4 The created vec4 object.
     */
    inline vec4 vec4_create(const f32 x, const f32 y, const f32 z, const f32 w)
    {
        return vec4 { x, y, z, w };
    }

    /**
     * @brief Create a vec4 object with zero values.
     * 
     * @return vec4 The created vec4 object.
     */
    inline vec4 vec4_zero()
    {
        return vec4 { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    /**
     * @brief Create a vec4 object with one values.
     * 
     * @return vec4 The created vec4 object.
     */
    inline vec4 vec4_one()
    {
        return vec4 { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    /**
     * @brief Add two vec4 objects.
     * 
     * @param a The first vec4 object.
     * @param b The second vec4 object.
     * @return vec4 The result of the addition.
     */
    inline vec4 vec4_add(const vec4& a, const vec4& b)
    {
        return vec4 { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    }

    /**
     * @brief Subtract two vec4 objects.
     * 
     * @param a The first vec4 object.
     * @param b The second vec4 object.
     * @return vec4 The result of the subtraction.
     */
    inline vec4 vec4_sub(const vec4& a, const vec4& b)
    {
        return vec4 { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    }

    /**
     * @brief Multiply two vec4 objects.
     * 
     * @param a The first vec4 object.
     * @param b The second vec4 object.
     * @return vec4 The result of the multiplication.
     */
    inline vec4 vec4_mul(const vec4& a, const vec4& b)
    {
        return vec4 { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
    }

    /**
     * @brief Divide two vec4 objects.
     * 
     * @param a The first vec4 object.
     * @param b The second vec4 object.
     * @return vec4 The result of the division.
     */
    inline vec4 vec4_div(const vec4& a, const vec4& b)
    {
        return vec4 { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
    }

    /**
     * @brief Scale a vec4 object by a scalar value.
     * 
     * @param a The vec4 object.
     * @param scale The scalar value.
     * @return vec4 The scaled vec4 object.
     */
    inline vec4 vec4_scale(const vec4& a, const f32 scale)
    {
        return vec4 { a.x * scale, a.y * scale, a.z * scale, a.w * scale };
    }

    // -------------------------------------------------------------
    // mat4x4 operations
    // -------------------------------------------------------------

    /**
     * @brief Create an identity mat4x4 object.
     * 
     * @return mat4x4 The created mat4x4 object.
     */
    inline mat4x4 mat4x4_id()
    {
        return mat4x4 {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    /**
     * @brief Create a zero mat4x4 object.
     * 
     * @return mat4x4 The created mat4x4 object.
     */
    inline mat4x4 mat4x4_zero()
    {
        return mat4x4 {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    /**
     * @brief Create a mat4x4 object with a specified value.
     * 
     * @param m The value for all the elements in the matrix.
     * @return mat4x4 The created mat4x4 object.
     */
    inline mat4x4 mat4x4_create(const f32 m)
    {
        return mat4x4 {
            m, 0.0f, 0.0f, 0.0f,
            0.0f, m, 0.0f, 0.0f,
            0.0f, 0.0f, m, 0.0f,
            0.0f, 0.0f, 0.0f, m
        };
    }

    /**
     * @brief Add two mat4x4 objects.
     * 
     * @param a The first mat4x4 object.
     * @param b The second mat4x4 object.
     * @return mat4x4 The result of the addition.
     */
    inline mat4x4 mat4x4_add(const mat4x4& a, const mat4x4& b)
    {
        return mat4x4 {
            a.elem[0][0] + b.elem[0][0], a.elem[0][1] + b.elem[0][1], a.elem[0][2] + b.elem[0][2], a.elem[0][3] + b.elem[0][3],
            a.elem[1][0] + b.elem[1][0], a.elem[1][1] + b.elem[1][1], a.elem[1][2] + b.elem[1][2], a.elem[1][3] + b.elem[1][3],
            a.elem[2][0] + b.elem[2][0], a.elem[2][1] + b.elem[2][1], a.elem[2][2] + b.elem[2][2], a.elem[2][3] + b.elem[2][3],
            a.elem[3][0] + b.elem[3][0], a.elem[3][1] + b.elem[3][1], a.elem[3][2] + b.elem[3][2], a.elem[3][3] + b.elem[3][3]
        };
    }

    /**
     * @brief Subtract two mat4x4 objects.
     * 
     * @param a The first mat4x4 object.
     * @param b The second mat4x4 object.
     * @return mat4x4 The result of the subtraction.
     */
    inline mat4x4 mat4x4_sub(const mat4x4& a, const mat4x4& b)
    {
        return mat4x4 {
            a.elem[0][0] - b.elem[0][0], a.elem[0][1] - b.elem[0][1], a.elem[0][2] - b.elem[0][2], a.elem[0][3] - b.elem[0][3],
            a.elem[1][0] - b.elem[1][0], a.elem[1][1] - b.elem[1][1], a.elem[1][2] - b.elem[1][2], a.elem[1][3] - b.elem[1][3],
            a.elem[2][0] - b.elem[2][0], a.elem[2][1] - b.elem[2][1], a.elem[2][2] - b.elem[2][2], a.elem[2][3] - b.elem[2][3],
            a.elem[3][0] - b.elem[3][0], a.elem[3][1] - b.elem[3][1], a.elem[3][2] - b.elem[3][2], a.elem[3][3] - b.elem[3][3]
        };
    }

    /**
     * @brief Multiply two mat4x4 objects.
     * 
     * @param a The first mat4x4 object.
     * @param b The second mat4x4 object.
     * @return mat4x4 The result of the multiplication.
     */
    inline mat4x4 mat4x4_mul(const mat4x4& a, const mat4x4& b)
    {
        return mat4x4 {
            a.elem[0][0] * b.elem[0][0] + a.elem[0][1] * b.elem[1][0] + a.elem[0][2] * b.elem[2][0] + a.elem[0][3] * b.elem[3][0],
            a.elem[0][0] * b.elem[0][1] + a.elem[0][1] * b.elem[1][1] + a.elem[0][2] * b.elem[2][1] + a.elem[0][3] * b.elem[3][1],
            a.elem[0][0] * b.elem[0][2] + a.elem[0][1] * b.elem[1][2] + a.elem[0][2] * b.elem[2][2] + a.elem[0][3] * b.elem[3][2],
            a.elem[0][0] * b.elem[0][3] + a.elem[0][1] * b.elem[1][3] + a.elem[0][2] * b.elem[2][3] + a.elem[0][3] * b.elem[3][3],

            a.elem[1][0] * b.elem[0][0] + a.elem[1][1] * b.elem[1][0] + a.elem[1][2] * b.elem[2][0] + a.elem[1][3] * b.elem[3][0],
            a.elem[1][0] * b.elem[0][1] + a.elem[1][1] * b.elem[1][1] + a.elem[1][2] * b.elem[2][1] + a.elem[1][3] * b.elem[3][1],
            a.elem[1][0] * b.elem[0][2] + a.elem[1][1] * b.elem[1][2] + a.elem[1][2] * b.elem[2][2] + a.elem[1][3] * b.elem[3][2],
            a.elem[1][0] * b.elem[0][3] + a.elem[1][1] * b.elem[1][3] + a.elem[1][2] * b.elem[2][3] + a.elem[1][3] * b.elem[3][3],

            a.elem[2][0] * b.elem[0][0] + a.elem[2][1] * b.elem[1][0] + a.elem[2][2] * b.elem[2][0] + a.elem[2][3] * b.elem[3][0],
            a.elem[2][0] * b.elem[0][1] + a.elem[2][1] * b.elem[1][1] + a.elem[2][2] * b.elem[2][1] + a.elem[2][3] * b.elem[3][1],
            a.elem[2][0] * b.elem[0][2] + a.elem[2][1] * b.elem[1][2] + a.elem[2][2] * b.elem[2][2] + a.elem[2][3] * b.elem[3][2],
            a.elem[2][0] * b.elem[0][3] + a.elem[2][1] * b.elem[1][3] + a.elem[2][2] * b.elem[2][3] + a.elem[2][3] * b.elem[3][3],

            a.elem[3][0] * b.elem[0][0] + a.elem[3][1] * b.elem[1][0] + a.elem[3][2] * b.elem[2][0] + a.elem[3][3] * b.elem[3][0],
            a.elem[3][0] * b.elem[0][1] + a.elem[3][1] * b.elem[1][1] + a.elem[3][2] * b.elem[2][1] + a.elem[3][3] * b.elem[3][1],
            a.elem[3][0] * b.elem[0][2] + a.elem[3][1] * b.elem[1][2] + a.elem[3][2] * b.elem[2][2] + a.elem[3][3] * b.elem[3][2],
            a.elem[3][0] * b.elem[0][3] + a.elem[3][1] * b.elem[1][3] + a.elem[3][2] * b.elem[2][3] + a.elem[3][3] * b.elem[3][3]
        };
    }

    /**
     * @brief Scale a mat4x4 object by a scalar value.
     * 
     * @param a The mat4x4 object.
     * @param scale The scalar value.
     * @return mat4x4 The scaled mat4x4 object.
     */
    inline mat4x4 mat4x4_scale_all(const mat4x4& a, const f32 scale)
    {
        return mat4x4 {
            a.elem[0][0] * scale, a.elem[0][1] * scale, a.elem[0][2] * scale, a.elem[0][3] * scale,
            a.elem[1][0] * scale, a.elem[1][1] * scale, a.elem[1][2] * scale, a.elem[1][3] * scale,
            a.elem[2][0] * scale, a.elem[2][1] * scale, a.elem[2][2] * scale, a.elem[2][3] * scale,
            a.elem[3][0] * scale, a.elem[3][1] * scale, a.elem[3][2] * scale, a.elem[3][3] * scale
        };
    }
}

#endif // _JOJ_JCMATH_H