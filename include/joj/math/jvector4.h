#ifndef _JOJ_VECTOR4_H
#define _JOJ_VECTOR4_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS
#include "DirectXMath.h"
#endif

namespace joj
{
    class JAPI JVector4
    {
    public:
        // Constructors
        JVector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        JVector4(f32 value) : x(value), y(value), z(value), w(value) {}
        JVector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

#if JPLATFORM_WINDOWS
        JVector4(const DirectX::XMFLOAT4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
#endif

        // Binary operators
        JVector4 operator+(const JVector4& other) const { return JVector4(x + other.x, y + other.y, z + other.z, w + other.w); }
        JVector4 operator-(const JVector4& other) const { return JVector4(x - other.x, y - other.y, z - other.z, w - other.w); }
        JVector4 operator*(const JVector4& other) const { return JVector4(x * other.x, y * other.y, z * other.z, w * other.w); }
        JVector4 operator/(const JVector4& other) const { return JVector4(x / other.x, y / other.y, z / other.z, w / other.w); }

        // Unary operators
        JVector4 operator+(f32 value) const { return JVector4(x + value, y + value, z + value, w + value); }
        JVector4 operator-(f32 value) const { return JVector4(x - value, y - value, z - value, w - value); }
        JVector4 operator*(f32 value) const { return JVector4(x * value, y * value, z * value, w * value); }
        JVector4 operator/(f32 value) const { return JVector4(x / value, y / value, z / value, w / value); }

        // Attribution operators
        JVector4& operator+=(const JVector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        JVector4& operator-=(const JVector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        JVector4& operator*=(const JVector4& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
        JVector4& operator/=(const JVector4& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }

        JVector4& operator+=(f32 value) { x += value; y += value; z += value; w += value; return *this; }
        JVector4& operator-=(f32 value) { x -= value; y -= value; z -= value; w -= value; return *this; }
        JVector4& operator*=(f32 value) { x *= value; y *= value; z *= value; w *= value; return *this; }
        JVector4& operator/=(f32 value) { x /= value; y /= value; z /= value; w /= value; return *this; }

        // Comparing operators
        bool operator==(const JVector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const JVector4& other) const { return !(*this == other); }

        // Vector methods
        f32 length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        f32 length_squared() const { return x * x + y * y + z * z + w * w; }
        f32 dot(const JVector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

        // Normalization
        JVector4 normalized() const
        {
            f32 len = length();
            return (len > 0) ? *this / len : JVector4();
        }

        void normalize()
        {
            f32 len = length();
            if (len > 0)
            {
                *this /= len;
            }
        }

        // Access to components as RGBA or STPQ
        union
        {
            struct { f32 x, y, z, w; }; // Vector components
            struct { f32 r, g, b, a; }; // RGBA components
            struct { f32 s, t, p, q; }; // STPQ components
        };

#if JPLATFORM_WINDOWS
        DirectX::XMFLOAT4 to_XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }
        void from_XMFLOAT4(const DirectX::XMFLOAT4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
        b8 operator==(const DirectX::XMFLOAT4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        b8 is_equal_to_XMFLOAT4(const DirectX::XMFLOAT4& other, f32 epsilon = 0.0001f) const
        {
            return std::abs(x - other.x) < epsilon &&
                std::abs(y - other.y) < epsilon &&
                std::abs(z - other.z) < epsilon &&
                std::abs(w - other.w) < epsilon;
        }
#endif
    };

#if JPLATFORM_WINDOWS
    DirectX::XMFLOAT4 JVector4_to_XMFLOAT4(const JVector4& v) { return DirectX::XMFLOAT4(v.x, v.y, v.z, v.w); }
    JVector4 XMFLOAT4_to_JVector4(const DirectX::XMFLOAT4& v) { return JVector4(v.x, v.y, v.z, v.w); }
    b8 is_JVector4_equal_to_XMFLOAT4(const JVector4& v1, const DirectX::XMFLOAT4& v2, f32 epsilon = 0.0001f)
    {
        return std::abs(v1.x - v2.x) < epsilon &&
            std::abs(v1.y - v2.y) < epsilon &&
            std::abs(v1.z - v2.z) < epsilon &&
            std::abs(v1.w - v2.w) < epsilon;
    }
#endif
}

#endif // _JOJ_VECTOR4_H