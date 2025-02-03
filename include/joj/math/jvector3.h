#ifndef _JOJ_VECTOR3_H
#define _JOJ_VECTOR3_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS
#include "DirectXMath.h"
#endif

namespace joj
{
    class JAPI JVector3
    {
    public:
        // Constructors
        JVector3() : x(0.0f), y(0.0f), z(0.0f) {}
        JVector3(f32 value) : x(value), y(value), z(value) {}
        JVector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

        // Binary operators
        JVector3 operator+(const JVector3& other) const { return JVector3(x + other.x, y + other.y, z + other.z); }
        JVector3 operator-(const JVector3& other) const { return JVector3(x - other.x, y - other.y, z - other.z); }
        JVector3 operator*(const JVector3& other) const { return JVector3(x * other.x, y * other.y, z * other.z); }
        JVector3 operator/(const JVector3& other) const { return JVector3(x / other.x, y / other.y, z / other.z); }

        // Unary operators
        JVector3 operator+(f32 value) const { return JVector3(x + value, y + value, z + value); }
        JVector3 operator-(f32 value) const { return JVector3(x - value, y - value, z - value); }
        JVector3 operator*(f32 value) const { return JVector3(x * value, y * value, z * value); }
        JVector3 operator/(f32 value) const { return JVector3(x / value, y / value, z / value); }

        // Attribution operators
        JVector3& operator+=(const JVector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        JVector3& operator-=(const JVector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        JVector3& operator*=(const JVector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
        JVector3& operator/=(const JVector3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

        JVector3& operator+=(f32 value) { x += value; y += value; z += value; return *this; }
        JVector3& operator-=(f32 value) { x -= value; y -= value; z -= value; return *this; }
        JVector3& operator*=(f32 value) { x *= value; y *= value; z *= value; return *this; }
        JVector3& operator/=(f32 value) { x /= value; y /= value; z /= value; return *this; }

        // Comparing operators
        bool operator==(const JVector3& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const JVector3& other) const { return !(*this == other); }

        // Vector methods
        f32 length() const { return std::sqrt(x * x + y * y + z * z); }
        f32 length_squared() const { return x * x + y * y + z * z; }
        f32 dot(const JVector3& other) const { return x * other.x + y * other.y + z * other.z; }
        JVector3 cross(const JVector3& other) const
        {
            return JVector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        // Normalization
        JVector3 normalized() const
        {
            f32 len = length();
            return (len > 0) ? *this / len : JVector3();
        }

        void normalize()
        {
            f32 len = length();
            if (len > 0)
            {
                *this /= len;
            }
        }

        // Access to components as RGB or UVW
        union
        {
            struct { f32 x, y, z; }; // Vector components
            struct { f32 r, g, b; }; // RGB components
            struct { f32 u, v, w; }; // Componentes UVW
        };

#if JPLATFORM_WINDOWS
        DirectX::XMFLOAT3 to_XMFLOAT3() const { return DirectX::XMFLOAT3(x, y, z); }
        void from_XMFLOAT3(const DirectX::XMFLOAT3& v) { x = v.x; y = v.y; z = v.z; }
#endif
    };

#if JPLATFORM_WINDOWS
    DirectX::XMFLOAT3 JVector3_to_XMFLOAT3(const JVector3& v) { return DirectX::XMFLOAT3(v.x, v.y, v.z); }
    JVector3 XMFLOAT3_to_JVector3(const DirectX::XMFLOAT3& v) { return JVector3(v.x, v.y, v.z); }
#endif
}

#endif // _JOJ_VECTOR3_H
