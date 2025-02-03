#include "joj/test/jtest_macros.h"
#include "joj/math/jvector4.h"
#include "joj/math/jmath.h"

#if JPLATFORM_WINDOWS
#include <DirectXMath.h>
#endif

JOJ_TEST(Constructor_Default)
{
    joj::JVector4 v;
    assert(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f && v.w == 0.0f);
}

JOJ_TEST(Constructor_Value)
{
    joj::JVector4 v(1.0f);
    assert(v.x == 1.0f && v.y == 1.0f && v.z == 1.0f && v.w == 1.0f);
}

JOJ_TEST(Constructor_Values)
{
    joj::JVector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f && v.w == 4.0f);
}

JOJ_TEST(Operator_Addition)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 v2(4.0f, 5.0f, 6.0f, 7.0f);
    joj::JVector4 result = v1 + v2;
    assert(result.x == 5.0f && result.y == 7.0f && result.z == 9.0f && result.w == 11.0f);
}

JOJ_TEST(Operator_Subtraction)
{
    joj::JVector4 v1(5.0f, 7.0f, 9.0f, 11.0f);
    joj::JVector4 v2(4.0f, 5.0f, 6.0f, 7.0f);
    joj::JVector4 result = v1 - v2;
    assert(result.x == 1.0f && result.y == 2.0f && result.z == 3.0f && result.w == 4.0f);
}

JOJ_TEST(Operator_Multiplication)
{
    joj::JVector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 result = v * 2.0f;
    assert(result.x == 2.0f && result.y == 4.0f && result.z == 6.0f && result.w == 8.0f);
}

JOJ_TEST(Operator_Division)
{
    joj::JVector4 v(6.0f, 9.0f, 12.0f, 15.0f);
    joj::JVector4 result = v / 3.0f;
    assert(result.x == 2.0f && result.y == 3.0f && result.z == 4.0f && result.w == 5.0f);
}

JOJ_TEST(Length)
{
    joj::JVector4 v(1.0f, 2.0f, 2.0f, 2.0f);
    assert(v.length() == 3.605551275f);
}

JOJ_TEST(Dot_Product)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 v2(4.0f, 3.0f, 2.0f, 1.0f);
    assert(v1.dot(v2) == 20.0f);
}

JOJ_TEST(Normalization)
{
    joj::JVector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 normalized = v.normalized();

    // Define expected values
    f32 expected_x = 0.182574f;
    f32 expected_y = 0.365148f;
    f32 expected_z = 0.547723f;
    f32 expected_w = 0.730297f;

    // Check if the normalized components are within tolerance
    assert(joj::are_floats_equal(normalized.x, expected_x));
    assert(joj::are_floats_equal(normalized.y, expected_y));
    assert(joj::are_floats_equal(normalized.z, expected_z));
    assert(joj::are_floats_equal(normalized.w, expected_w));
}

#if JPLATFORM_WINDOWS
JOJ_TEST(Conversion_To_XMFLOAT4)
{
    joj::JVector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    DirectX::XMFLOAT4 dxv = v.to_XMFLOAT4();
    assert(dxv.x == 1.0f && dxv.y == 2.0f && dxv.z == 3.0f && dxv.w == 4.0f);
}

JOJ_TEST(Conversion_From_XMFLOAT4)
{
    DirectX::XMFLOAT4 dxv = { 1.0f, 2.0f, 3.0f, 4.0f };
    joj::JVector4 v;
    v.from_XMFLOAT4(dxv);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f && v.w == 4.0f);
}

JOJ_TEST(Equality_Method_Check)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    DirectX::XMFLOAT4 dxv2 = { 1.0f, 2.0f, 3.0f, 4.0f };
    assert(v1.is_equal_to_XMFLOAT4(dxv2));
}

JOJ_TEST(Equality_Function_Check)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    DirectX::XMFLOAT4 dxv2 = { 1.0f, 2.0f, 3.0f, 4.0f };
    assert(is_JVector4_equal_to_XMFLOAT4(v1, dxv2));
}

JOJ_TEST(Vector_Addition)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 v2(4.0f, 5.0f, 6.0f, 7.0f);

    DirectX::XMFLOAT4 dxv1 = v1.to_XMFLOAT4();
    DirectX::XMFLOAT4 dxv2 = v2.to_XMFLOAT4();

    DirectX::XMFLOAT4 dxSum = { dxv1.x + dxv2.x, dxv1.y + dxv2.y, dxv1.z + dxv2.z, dxv1.w + dxv2.w };
    joj::JVector4 jSum = v1 + v2;

    assert(is_JVector4_equal_to_XMFLOAT4(jSum, dxSum));
}

JOJ_TEST(Vector_Normalization)
{
    joj::JVector4 v(3.0f, 4.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT4 dxv = v.to_XMFLOAT4();

    // Normalizar usando DirectX
    DirectX::XMVECTOR dxVectorNorm = DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&dxv));

    DirectX::XMFLOAT4 dxNorm;
    DirectX::XMStoreFloat4(&dxNorm, dxVectorNorm);

    // Normalizar usando a função personalizada
    joj::JVector4 jNorm = v.normalized();

    assert(is_JVector4_equal_to_XMFLOAT4(jNorm, dxNorm));
}

JOJ_TEST(Vector_Dot_Product)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    joj::JVector4 v2(4.0f, 5.0f, 6.0f, 7.0f);

    DirectX::XMFLOAT4 dxv1 = v1.to_XMFLOAT4();
    DirectX::XMFLOAT4 dxv2 = v2.to_XMFLOAT4();

    f32 dxDot = DirectX::XMVector4Dot(DirectX::XMLoadFloat4(&dxv1), DirectX::XMLoadFloat4(&dxv2)).m128_f32[0];
    f32 jDot = v1.dot(v2);

    assert(dxDot == jDot);
}

JOJ_TEST(Vector_Equality_Check)
{
    joj::JVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    DirectX::XMFLOAT4 dxv2 = { 1.0f, 2.0f, 3.0f, 4.0f };

    assert(v1 == dxv2);
}
#endif

int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}
