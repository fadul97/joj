#ifndef _JOJ_MATH_H
#define _JOJ_MATH_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <DirectXMath.h>
#include <iostream>
#include <cmath>
#include "vector3.h"

#define J_PI DirectX::XM_PI
#define J_2PI DirectX::XM_2PI
#define JINFINITY FLT_MAX

namespace joj
{
    // Vector types
    struct JInt4
    {
        u32 x, y, z, w;
    };
    
    using JFloat2 = DirectX::XMFLOAT2;
    using JFloat3 = DirectX::XMFLOAT3;
    using JFloat4 = DirectX::XMFLOAT4;
    using JXMVector4 = DirectX::XMVECTOR;

    // Matrix types
    using JFloat4x4 = DirectX::XMFLOAT4X4;
    using JMatrix4x4 = DirectX::XMMATRIX;

    inline JFloat4x4 float4x4_identity()
    {
        static JFloat4x4 I(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return I;
    }

    inline void float4x4_print(const JFloat4x4& f)
    {
        std::cout << f.m[0][0] << " " << f.m[0][1] << " " << f.m[0][2] << " " << f.m[0][3] << std::endl;
        std::cout << f.m[1][0] << " " << f.m[1][1] << " " << f.m[1][2] << " " << f.m[1][3] << std::endl;
        std::cout << f.m[2][0] << " " << f.m[2][1] << " " << f.m[2][2] << " " << f.m[2][3] << std::endl;
        std::cout << f.m[3][0] << " " << f.m[3][1] << " " << f.m[3][2] << " " << f.m[3][3] << std::endl;
    }

    inline JMatrix4x4 matrix4x4_identity()
    {
        static JMatrix4x4 I(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return I;
    }

    inline JMatrix4x4 inverse_transpose(const JMatrix4x4& M)
    {
        // Inverse-transpose is just applied to normals.  So zero out 
        // translation row so that it doesn't get into our inverse-transpose
        // calculation--we don't want the inverse-transpose of the translation.
        JMatrix4x4 A = M;
        A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

        JXMVector4 det = DirectX::XMMatrixDeterminant(A);
        return DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A));
    }

    template<typename T>
    inline T Min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    inline T Max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    inline JFloat4 calculate_tangent(const JFloat3& edge1, const JFloat3& edge2,
        const JFloat2& deltaUV1, const JFloat2& deltaUV2)
    {
        f32 f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        JFloat3 tangent =
        {
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };
        
        return { tangent.x, tangent.y, tangent.z, 1.0f };
    }

    inline b8 are_floats_equal(const f32 a, const f32 b, const f32 epsilon = 0.0001f)
    {
        return std::fabs(a - b) <= epsilon;
    }

    /**
     * @brief Linear interpolation between two vectors.
     * 
     * @param a The first vector.
     * @param b The second vector.
     * @param t The interpolation factor.
     * @return Vector3 The interpolated vector.
     */
    inline Vector3 lerp(const Vector3& a, const Vector3& b, const f32 t)
    {
        return {
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
        };
    }
}

#elif JOJ_PLATFORM_LINUX

#include <cmath>

namespace joj
{
    inline b8 are_floats_equal(const f32 a, const f32 b, const f32 epsilon = 0.0001f)
    {
        return std::fabs(a - b) <= epsilon;
    }
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_MATH_H