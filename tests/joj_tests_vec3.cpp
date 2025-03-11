#include "joj/test/jtest_macros.h"
#include "joj/math/jvector3.h"

#if JOJ_PLATFORM_WINDOWS
#include <DirectXMath.h>
#endif

JOJ_TEST(Constructor_Default)
{
    joj::JVector3 v;
    assert(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);
}

JOJ_TEST(Constructor_Value)
{
    joj::JVector3 v(1.0f);
    assert(v.x == 1.0f && v.y == 1.0f && v.z == 1.0f);
}

JOJ_TEST(Constructor_Values)
{
    joj::JVector3 v(1.0f, 2.0f, 3.0f);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f);
}

JOJ_TEST(Operator_Addition)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);
    joj::JVector3 result = v1 + v2;
    assert(result.x == 5.0f && result.y == 7.0f && result.z == 9.0f);
}

JOJ_TEST(Operator_Subtraction)
{
    joj::JVector3 v1(5.0f, 7.0f, 9.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);
    joj::JVector3 result = v1 - v2;
    assert(result.x == 1.0f && result.y == 2.0f && result.z == 3.0f);
}

JOJ_TEST(Operator_Multiplication)
{
    joj::JVector3 v(1.0f, 2.0f, 3.0f);
    joj::JVector3 result = v * 2.0f;
    assert(result.x == 2.0f && result.y == 4.0f && result.z == 6.0f);
}

JOJ_TEST(Operator_Division)
{
    joj::JVector3 v(6.0f, 9.0f, 12.0f);
    joj::JVector3 result = v / 3.0f;
    assert(result.x == 2.0f && result.y == 3.0f && result.z == 4.0f);
}

JOJ_TEST(Length)
{
    joj::JVector3 v(3.0f, 4.0f, 0.0f);
    assert(v.length() == 5.0f);
}

JOJ_TEST(Dot_Product)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);
    assert(v1.dot(v2) == 32.0f);
}

JOJ_TEST(Cross_Product)
{
    joj::JVector3 v1(1.0f, 0.0f, 0.0f);
    joj::JVector3 v2(0.0f, 1.0f, 0.0f);
    joj::JVector3 result = v1.cross(v2);
    assert(result.x == 0.0f && result.y == 0.0f && result.z == 1.0f);
}

JOJ_TEST(Normalization)
{
    joj::JVector3 v(3.0f, 4.0f, 0.0f);
    joj::JVector3 normalized = v.normalized();
    assert(normalized.x == 0.6f && normalized.y == 0.8f && normalized.z == 0.0f);
}

#if JOJ_PLATFORM_WINDOWS
JOJ_TEST(Conversion_To_XMFLOAT3)
{
    joj::JVector3 v(1.0f, 2.0f, 3.0f);
    DirectX::XMFLOAT3 dxv = v.to_XMFLOAT3();
    assert(dxv.x == 1.0f && dxv.y == 2.0f && dxv.z == 3.0f);
}

JOJ_TEST(Conversion_From_XMFLOAT3)
{
    DirectX::XMFLOAT3 dxv = { 1.0f, 2.0f, 3.0f };
    joj::JVector3 v;
    v.from_XMFLOAT3(dxv);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f);
}

JOJ_TEST(Equality_Method_Check)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    DirectX::XMFLOAT3 dxv2 = { 1.0f, 2.0f, 3.0f };
    assert(v1.is_equal_to_XMFLOAT3(dxv2));
}

JOJ_TEST(Equality_Function_Check)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    DirectX::XMFLOAT3 dxv2 = { 1.0f, 2.0f, 3.0f };
    assert(is_JVector3_equal_to_XMFLOAT3(v1, dxv2));
}

JOJ_TEST(Vector_Addition)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);

    DirectX::XMFLOAT3 dxv1 = v1.to_XMFLOAT3();
    DirectX::XMFLOAT3 dxv2 = v2.to_XMFLOAT3();

    DirectX::XMFLOAT3 dxSum = { dxv1.x + dxv2.x, dxv1.y + dxv2.y, dxv1.z + dxv2.z };
    joj::JVector3 jSum = v1 + v2;

    assert(is_JVector3_equal_to_XMFLOAT3(jSum, dxSum));
}

JOJ_TEST(Vector_Normalization)
{
    joj::JVector3 v(3.0f, 4.0f, 0.0f);
    DirectX::XMFLOAT3 dxv = v.to_XMFLOAT3();

    // Normalizar usando DirectX
    DirectX::XMVECTOR dxVectorNorm = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dxv));

    DirectX::XMFLOAT3 dxNorm;
    DirectX::XMStoreFloat3(&dxNorm, dxVectorNorm);

    // Normalizar usando a fun��o personalizada
    joj::JVector3 jNorm = v.normalized();

    assert(is_JVector3_equal_to_XMFLOAT3(jNorm, dxNorm));
}

JOJ_TEST(Vector_Cross_Product)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);

    DirectX::XMFLOAT3 dxv1 = v1.to_XMFLOAT3();
    DirectX::XMFLOAT3 dxv2 = v2.to_XMFLOAT3();

    DirectX::XMVECTOR dxVectorCross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&dxv1), DirectX::XMLoadFloat3(&dxv2));

    DirectX::XMFLOAT3 dxCross;
    DirectX::XMStoreFloat3(&dxCross, dxVectorCross);

    joj::JVector3 jCross = v1.cross(v2);

    assert(is_JVector3_equal_to_XMFLOAT3(jCross, dxCross));
}

JOJ_TEST(Vector_Dot_Product)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    joj::JVector3 v2(4.0f, 5.0f, 6.0f);

    DirectX::XMFLOAT3 dxv1 = v1.to_XMFLOAT3();
    DirectX::XMFLOAT3 dxv2 = v2.to_XMFLOAT3();

    f32 dxDot = DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&dxv1), DirectX::XMLoadFloat3(&dxv2)).m128_f32[0];
    f32 jDot = v1.dot(v2);

    assert(dxDot == jDot);
}

JOJ_TEST(Vector_Equality_Check)
{
    joj::JVector3 v1(1.0f, 2.0f, 3.0f);
    DirectX::XMFLOAT3 dxv2 = { 1.0f, 2.0f, 3.0f };

    assert(v1 == dxv2);
}
#endif

int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}