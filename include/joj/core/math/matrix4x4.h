#ifndef _JOJ_MATRIX_4X4_H
#define _JOJ_MATRIX_4X4_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS
#include "DirectXMath.h"
#endif

#include <cmath>
#include "vector3.h"

namespace joj
{
    class JOJ_API Matrix4x4
    {
    public:
        // Constructors
        Matrix4x4() : m{ 0 } {}
        Matrix4x4(f32 value) { for (int i = 0; i < 16; ++i) m[i] = value; }
        Matrix4x4(const f32* values) { for (int i = 0; i < 16; ++i) m[i] = values[i]; }
        Matrix4x4(const Matrix4x4& other) { for (int i = 0; i < 16; ++i) m[i] = other.m[i]; }

        // Accessors
        f32& operator[](int index) { return m[index]; }
        const f32& operator[](int index) const { return m[index]; }

        // Operators
        Matrix4x4 operator+(const Matrix4x4& other) const;
        Matrix4x4 operator-(const Matrix4x4& other) const;
        Matrix4x4 operator*(const Matrix4x4& other) const;
        Matrix4x4 operator*(f32 scalar) const;
        Matrix4x4 operator/(f32 scalar) const;
        Matrix4x4& operator=(const Matrix4x4& other) { for (int i = 0; i < 16; ++i) m[i] = other.m[i]; return *this; }
        Matrix4x4& operator+=(const Matrix4x4& other) { for (int i = 0; i < 16; ++i) m[i] += other.m[i]; return *this; }
        Matrix4x4& operator-=(const Matrix4x4& other) { for (int i = 0; i < 16; ++i) m[i] -= other.m[i]; return *this; }
        Matrix4x4& operator*=(const Matrix4x4& other) { *this = *this * other; return *this; }
        Matrix4x4& operator*=(f32 scalar) { for (int i = 0; i < 16; ++i) m[i] *= scalar; return *this; }
        Matrix4x4& operator/=(f32 scalar) { for (int i = 0; i < 16; ++i) m[i] /= scalar; return *this; }

        bool operator==(const Matrix4x4& other) const { for (int i = 0; i < 16; ++i) if (m[i] != other.m[i]) return false; return true; }
        bool operator!=(const Matrix4x4& other) const { return !(*this == other); }
        
        b8 is_equal(const Matrix4x4& other, f32 epsilon = 0.0001f) const
        {
            for (int i = 0; i < 16; ++i)
            {
                if (std::abs(m[i] - other.m[i]) > epsilon)
                    return false;
            }
            return true;
        }

        Matrix4x4(const f32 m0, const f32 m1, const f32 m2, const f32 m3,
            const f32 m4, const f32 m5, const f32 m6, const f32 m7,
            const f32 m8, const f32 m9, const f32 m10, const f32 m11,
            const f32 m12, const f32 m13, const f32 m14, const f32 m15)
        {
            m[0] = m0;   m[1] = m1;   m[2] = m2;   m[3] = m3;
            m[4] = m4;   m[5] = m5;   m[6] = m6;   m[7] = m7;
            m[8] = m8;   m[9] = m9;   m[10] = m10; m[11] = m11;
            m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
        }

        // Matrix operations
        Matrix4x4 transpose() const
        {
            return Matrix4x4
            {
                m[0], m[4], m[8],  m[12],
                m[1], m[5], m[9],  m[13],
                m[2], m[6], m[10], m[14],
                m[3], m[7], m[11], m[15]
            };
        }

        f32 m[16]; // Column-major order

#if JOJ_PLATFORM_WINDOWS
        DirectX::XMMATRIX to_XMMATRIX() const { return DirectX::XMMATRIX(m[0], m[1], m[2], m[3],
                                                                           m[4], m[5], m[6], m[7],
                                                                           m[8], m[9], m[10], m[11],
                                                                           m[12], m[13], m[14], m[15]); }
        void from_XMMATRIX(const DirectX::XMMATRIX& other)
        {
            m[0] = other.r[0].m128_f32[0]; m[1] = other.r[0].m128_f32[1]; m[2] = other.r[0].m128_f32[2]; m[3] = other.r[0].m128_f32[3];
            m[4] = other.r[1].m128_f32[0]; m[5] = other.r[1].m128_f32[1]; m[6] = other.r[1].m128_f32[2]; m[7] = other.r[1].m128_f32[3];
            m[8] = other.r[2].m128_f32[0]; m[9] = other.r[2].m128_f32[1]; m[10] = other.r[2].m128_f32[2]; m[11] = other.r[2].m128_f32[3];
            m[12] = other.r[3].m128_f32[0]; m[13] = other.r[3].m128_f32[1]; m[14] = other.r[3].m128_f32[2]; m[15] = other.r[3].m128_f32[3];
        }
        b8 operator==(const DirectX::XMMATRIX& other) const
        {
            return m[0] == other.r[0].m128_f32[0] && m[1] == other.r[0].m128_f32[1] && m[2] == other.r[0].m128_f32[2] && m[3] == other.r[0].m128_f32[3] &&
                   m[4] == other.r[1].m128_f32[0] && m[5] == other.r[1].m128_f32[1] && m[6] == other.r[1].m128_f32[2] && m[7] == other.r[1].m128_f32[3] &&
                   m[8] == other.r[2].m128_f32[0] && m[9] == other.r[2].m128_f32[1] && m[10] == other.r[2].m128_f32[2] && m[11] == other.r[2].m128_f32[3] &&
                   m[12] == other.r[3].m128_f32[0] && m[13] == other.r[3].m128_f32[1] && m[14] == other.r[3].m128_f32[2] && m[15] == other.r[3].m128_f32[3];
        }
        b8 is_equal_to_XMMATRIX(const DirectX::XMMATRIX& other, f32 epsilon = 0.0001f) const
        {
            return std::abs(m[0] - other.r[0].m128_f32[0]) < epsilon &&
                   std::abs(m[1] - other.r[0].m128_f32[1]) < epsilon &&
                   std::abs(m[2] - other.r[0].m128_f32[2]) < epsilon &&
                   std::abs(m[3] - other.r[0].m128_f32[3]) < epsilon &&
                   std::abs(m[4] - other.r[1].m128_f32[0]) < epsilon &&
                   std::abs(m[5] - other.r[1].m128_f32[1]) < epsilon &&
                   std::abs(m[6] - other.r[1].m128_f32[2]) < epsilon &&
                   std::abs(m[7] - other.r[1].m128_f32[3]) < epsilon &&
                   std::abs(m[8] - other.r[2].m128_f32[0]) < epsilon &&
                   std::abs(m[9] - other.r[2].m128_f32[1]) < epsilon &&
                   std::abs(m[10] - other.r[2].m128_f32[2]) < epsilon &&
                   std::abs(m[11] - other.r[2].m128_f32[3]) < epsilon &&
                   std::abs(m[12] - other.r[3].m128_f32[0]) < epsilon &&
                   std::abs(m[13] - other.r[3].m128_f32[1]) < epsilon &&
                   std::abs(m[14] - other.r[3].m128_f32[2]) < epsilon &&
                   std::abs(m[15] - other.r[3].m128_f32[3]) < epsilon;
        }
        void from_XMVECTOR(const DirectX::FXMVECTOR& v)
        {
            m[0] = DirectX::XMVectorGetX(v);
            m[1] = DirectX::XMVectorGetY(v);
            m[2] = DirectX::XMVectorGetZ(v);
            m[3] = DirectX::XMVectorGetW(v);
        }
#endif // JOJ_PLATFORM_WINDOWS

        // Static methods
        static Matrix4x4 identity()
        {
            return Matrix4x4
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4x4 zero()
        {
            return Matrix4x4
            {
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
            };
        }
        static Matrix4x4 translation(f32 x, f32 y, f32 z)
        {
            return Matrix4x4
            {
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
            };
        }

        static Matrix4x4 rotationX(f32 angle)
        {
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            return Matrix4x4
            {
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1
            };
        }
        static Matrix4x4 rotationY(f32 angle)
        {
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            return Matrix4x4
            {
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4x4 rotationZ(f32 angle)
        {
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            return Matrix4x4
            {
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4x4 scale(f32 x, f32 y, f32 z)
        {
            return Matrix4x4
            {
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4x4 perspective(f32 fov, f32 aspect, f32 near, f32 far)
        {
            f32 f = 1.0f / std::tan(fov / 2.0f);
            return Matrix4x4
            {
                f / aspect, 0, 0, 0,
                0, f, 0, 0,
                0, 0, (far + near) / (near - far), (2 * far * near) / (near - far),
                0, 0, -1, 0
            };
        }

        static Matrix4x4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
        {
            return Matrix4x4
            {
                2 / (right - left), 0, 0, -(right + left) / (right - left),
                0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                0, 0, -2 / (far - near), -(far + near) / (far - near),
                0, 0, 0, 1
            };
        }

        static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
        {
            Vector3 zaxis = (eye - target).normalized();
            Vector3 xaxis = up.cross(zaxis).normalized();
            Vector3 yaxis = zaxis.cross(xaxis);

            return Matrix4x4
            {
                xaxis.x, yaxis.x, zaxis.x, 0,
                xaxis.y, yaxis.y, zaxis.y, 0,
                xaxis.z, yaxis.z, zaxis.z, 0,
                -xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1
            };
        }

        static Matrix4x4 lookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up)
        {
            Vector3 zaxis = (eye - target).normalized();
            Vector3 xaxis = up.cross(zaxis).normalized();
            Vector3 yaxis = zaxis.cross(xaxis);

            return Matrix4x4
            {
                xaxis.x, yaxis.x, zaxis.x, 0,
                xaxis.y, yaxis.y, zaxis.y, 0,
                xaxis.z, yaxis.z, zaxis.z, 0,
                -xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1
            };
        }

        static Matrix4x4 lookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up)
        {
            Vector3 zaxis = (target - eye).normalized();
            Vector3 xaxis = up.cross(zaxis).normalized();
            Vector3 yaxis = zaxis.cross(xaxis);

            return Matrix4x4
            {
                xaxis.x, yaxis.x, zaxis.x, 0,
                xaxis.y, yaxis.y, zaxis.y, 0,
                xaxis.z, yaxis.z, zaxis.z, 0,
                -xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1
            };
        }

        static Matrix4x4 lookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up, f32 fov, f32 aspect, f32 near, f32 far)
        {
            return lookAtLH(eye, target, up) * perspective(fov, aspect, near, far);
        }

        static Matrix4x4 lookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up, f32 fov, f32 aspect, f32 near, f32 far)
        {
            return lookAtRH(eye, target, up) * perspective(fov, aspect, near, far);
        }
    };

#if JOJ_PLATFORM_WINDOWS
    inline DirectX::XMFLOAT4X4 to_XMFLOAT4X4(const Matrix4x4& m)
    {
        return DirectX::XMFLOAT4X4(m.m[0], m.m[1], m.m[2], m.m[3],
                                   m.m[4], m.m[5], m.m[6], m.m[7],
                                   m.m[8], m.m[9], m.m[10], m.m[11],
                                   m.m[12], m.m[13], m.m[14], m.m[15]);
    }
    inline Matrix4x4 from_XMFLOAT4X4(const DirectX::XMFLOAT4X4& m)
    {
        return Matrix4x4
        {
            m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
            m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
            m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
            m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
        };
    }
    inline Matrix4x4 from_XMMATRIX(const DirectX::XMMATRIX& m)
    {
        return Matrix4x4(m.r[0].m128_f32[0], m.r[0].m128_f32[1], m.r[0].m128_f32[2], m.r[0].m128_f32[3],
                         m.r[1].m128_f32[0], m.r[1].m128_f32[1], m.r[1].m128_f32[2], m.r[1].m128_f32[3],
                         m.r[2].m128_f32[0], m.r[2].m128_f32[1], m.r[2].m128_f32[2], m.r[2].m128_f32[3],
                         m.r[3].m128_f32[0], m.r[3].m128_f32[1], m.r[3].m128_f32[2], m.r[3].m128_f32[3]);
    }
    inline DirectX::XMMATRIX to_XMMATRIX(const Matrix4x4& m)
    {
        return DirectX::XMMATRIX(m.m[0], m.m[1], m.m[2], m.m[3],
                                 m.m[4], m.m[5], m.m[6], m.m[7],
                                 m.m[8], m.m[9], m.m[10], m.m[11],
                                 m.m[12], m.m[13], m.m[14], m.m[15]);
    }
    inline Matrix4x4 to_Matrix4x4(const DirectX::XMMATRIX& m)
    {
        return Matrix4x4(m.r[0].m128_f32[0], m.r[0].m128_f32[1], m.r[0].m128_f32[2], m.r[0].m128_f32[3],
                         m.r[1].m128_f32[0], m.r[1].m128_f32[1], m.r[1].m128_f32[2], m.r[1].m128_f32[3],
                         m.r[2].m128_f32[0], m.r[2].m128_f32[1], m.r[2].m128_f32[2], m.r[2].m128_f32[3],
                         m.r[3].m128_f32[0], m.r[3].m128_f32[1], m.r[3].m128_f32[2], m.r[3].m128_f32[3]);
    }
    inline Matrix4x4 to_Matrix4x4(const DirectX::XMFLOAT4X4& m)
    {
        return Matrix4x4(m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
                         m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
                         m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
                         m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
    }
#endif // JOJ_PLATFORM_WINDOWS
}

#endif // _JOJ_MATRIX_4X4_H