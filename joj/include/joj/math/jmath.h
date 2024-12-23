#ifndef _JOJ_MATH_H
#define _JOJ_MATH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <DirectXMath.h>
#include <iostream>

#define J_PI DirectX::XM_PI
#define J_2PI DirectX::XM_2PI
#define JINFINITY FLT_MAX

namespace joj
{
    // Vector types
    using JFloat2 = DirectX::XMFLOAT2;
    using JFloat3 = DirectX::XMFLOAT3;
    using JFloat4 = DirectX::XMFLOAT4;
    using JVector4 = DirectX::XMVECTOR;

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

        JVector4 det = DirectX::XMMatrixDeterminant(A);
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
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_MATH_H