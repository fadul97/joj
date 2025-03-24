#ifndef _JOJ_VECTOR4_H
#define _JOJ_VECTOR4_H

#include "joj/core/defines.h"

#include <cmath>

#if JOJ_PLATFORM_WINDOWS
#include "DirectXMath.h"
#endif

namespace joj
{
    class JOJ_API Vector4
    {
    public:
        // Constructors
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4(f32 value) : x(value), y(value), z(value), w(value) {}
        Vector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

#if JOJ_PLATFORM_WINDOWS
        Vector4(const DirectX::XMFLOAT4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
        Vector4(const DirectX::XMVECTOR& v) : x(DirectX::XMVectorGetX(v)), y(DirectX::XMVectorGetY(v)), z(DirectX::XMVectorGetZ(v)), w(DirectX::XMVectorGetW(v)) {}
#endif

        // Binary operators
        Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
        Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
        Vector4 operator*(const Vector4& other) const { return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
        Vector4 operator/(const Vector4& other) const { return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }

        // Unary operators
        Vector4 operator+(f32 value) const { return Vector4(x + value, y + value, z + value, w + value); }
        Vector4 operator-(f32 value) const { return Vector4(x - value, y - value, z - value, w - value); }
        Vector4 operator*(f32 value) const { return Vector4(x * value, y * value, z * value, w * value); }
        Vector4 operator/(f32 value) const { return Vector4(x / value, y / value, z / value, w / value); }

        // Attribution operators
        Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        Vector4& operator*=(const Vector4& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
        Vector4& operator/=(const Vector4& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }

        Vector4& operator+=(f32 value) { x += value; y += value; z += value; w += value; return *this; }
        Vector4& operator-=(f32 value) { x -= value; y -= value; z -= value; w -= value; return *this; }
        Vector4& operator*=(f32 value) { x *= value; y *= value; z *= value; w *= value; return *this; }
        Vector4& operator/=(f32 value) { x /= value; y /= value; z /= value; w /= value; return *this; }

        // Comparing operators
        bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const Vector4& other) const { return !(*this == other); }

        // Vector methods
        f32 length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        f32 length_squared() const { return x * x + y * y + z * z + w * w; }
        f32 dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

        // Normalization
        Vector4 normalized() const
        {
            f32 len = length();
            return (len > 0) ? *this / len : Vector4();
        }

        void normalize()
        {
            f32 len = length();
            if (len > 0)
            {
                *this /= len;
            }
        }

        // TODO: Union to access to components as RGBA or STPQ
        f32 x;
        f32 y;
        f32 z;
        f32 w;

#if JOJ_PLATFORM_WINDOWS
        // DirectX::XMFLOAT4
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

        // DirectX::XMVECTOR
        DirectX::XMVECTOR to_XMVECTOR() const { return DirectX::XMVectorSet(x, y, z, w); }
        void from_XMVECTOR(const DirectX::XMVECTOR& v) { x = DirectX::XMVectorGetX(v); y = DirectX::XMVectorGetY(v); z = DirectX::XMVectorGetZ(v); w = DirectX::XMVectorGetW(v); }
        b8 operator==(const DirectX::XMVECTOR& other) const { return x == DirectX::XMVectorGetX(other) && y == DirectX::XMVectorGetY(other) && z == DirectX::XMVectorGetZ(other) && w == DirectX::XMVectorGetW(other); }
        b8 is_equal_to_XMVECTOR(const DirectX::XMVECTOR& other, f32 epsilon = 0.0001f) const
        {
            return std::abs(x - DirectX::XMVectorGetX(other)) < epsilon &&
                std::abs(y - DirectX::XMVectorGetY(other)) < epsilon &&
                std::abs(z - DirectX::XMVectorGetZ(other)) < epsilon &&
                std::abs(w - DirectX::XMVectorGetW(other)) < epsilon;
        }
#endif
    };

#if JOJ_PLATFORM_WINDOWS
    // DirectX::XMFLOAT4
    inline DirectX::XMFLOAT4 Vector4_to_XMFLOAT4(const Vector4& v) { return DirectX::XMFLOAT4(v.x, v.y, v.z, v.w); }
    inline Vector4 XMFLOAT4_to_Vector4(const DirectX::XMFLOAT4& v) { return Vector4(v.x, v.y, v.z, v.w); }
    inline b8 is_Vector4_equal_to_XMFLOAT4(const Vector4& v1, const DirectX::XMFLOAT4& v2, f32 epsilon = 0.0001f)
    {
        return std::abs(v1.x - v2.x) < epsilon &&
            std::abs(v1.y - v2.y) < epsilon &&
            std::abs(v1.z - v2.z) < epsilon &&
            std::abs(v1.w - v2.w) < epsilon;
    }

    // DirectX::XMVECTOR
    inline DirectX::XMVECTOR Vector4_to_XMVECTOR(const Vector4& v) { return DirectX::XMVectorSet(v.x, v.y, v.z, v.w); }
    inline Vector4 XMVECTOR_to_Vector4(const DirectX::XMVECTOR& v) { return Vector4(DirectX::XMVectorGetX(v), DirectX::XMVectorGetY(v), DirectX::XMVectorGetZ(v), DirectX::XMVectorGetW(v)); }
    inline b8 is_Vector4_equal_to_XMVECTOR(const Vector4& v1, const DirectX::XMVECTOR& v2, f32 epsilon = 0.0001f)
    {
        return std::abs(v1.x - DirectX::XMVectorGetX(v2)) < epsilon &&
            std::abs(v1.y - DirectX::XMVectorGetY(v2)) < epsilon &&
            std::abs(v1.z - DirectX::XMVectorGetZ(v2)) < epsilon &&
            std::abs(v1.w - DirectX::XMVectorGetW(v2)) < epsilon;
    }
#endif
}

#endif // _JOJ_VECTOR4_H