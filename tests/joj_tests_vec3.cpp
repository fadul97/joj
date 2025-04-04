#include "joj/test/jtest_macros.h"
#include "joj/core/math/math_types.h"
#include "joj/core/math/jcmath.h"


JOJ_TEST(vec3_create_zero)
{
    joj::vec3 v = joj::vec3_create(0.0f, 0.0f, 0.0f);
    assert(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);

    v = joj::vec3_zero();
    assert(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);
}

JOJ_TEST(vec3_create_one)
{
    joj::vec3 v = joj::vec3_create(1.0f, 1.0f, 1.0f);
    assert(v.x == 1.0f && v.y == 1.0f && v.z == 1.0f);

    v = joj::vec3_one();
    assert(v.x == 1.0f && v.y == 1.0f && v.z == 1.0f);
}

JOJ_TEST(vec3_create_values)
{
    joj::vec3 v = joj::vec3_create(1.0f, 2.0f, 3.0f);
    assert(v.x == 1.0f && v.y == 2.0f && v.z == 3.0f);
}

JOJ_TEST(vec3_add)
{
    joj::vec3 v1 = joj::vec3_create(1.0f, 2.0f, 3.0f);
    joj::vec3 v2 = joj::vec3_create(4.0f, 5.0f, 6.0f);
    joj::vec3 result = joj::vec3_add(v1, v2);
    assert(result.x == 5.0f && result.y == 7.0f && result.z == 9.0f);

    joj::vec3 v4 = joj::vec3_create(10.0f, 20.0f, 30.0f);
    joj::vec3 v5 = joj::vec3_create(40.0f, 50.0f, 60.0f);
    joj::vec3 result2 = joj::vec3_add(v4, v5);
    assert(result2.x == 50.0f && result2.y == 70.0f && result2.z == 90.0f);
}

JOJ_TEST(vec3_sub)
{
    joj::vec3 v1 = joj::vec3_create(5.0f, 7.0f, 9.0f);
    joj::vec3 v2 = joj::vec3_create(4.0f, 5.0f, 6.0f);
    joj::vec3 result = joj::vec3_sub(v1, v2);
    assert(result.x == 1.0f && result.y == 2.0f && result.z == 3.0f);

    joj::vec3 v4 = joj::vec3_create(10.0f, 20.0f, 30.0f);
    joj::vec3 v5 = joj::vec3_create(40.0f, 50.0f, 60.0f);
    joj::vec3 result2 = joj::vec3_sub(v4, v5);
    assert(result2.x == -30.0f && result2.y == -30.0f && result2.z == -30.0f);
}

JOJ_TEST(vec3_mul)
{
    joj::vec3 v1 = joj::vec3_create(1.0f, 2.0f, 3.0f);
    joj::vec3 v2 = joj::vec3_create(4.0f, 5.0f, 6.0f);
    joj::vec3 result = joj::vec3_mul(v1, v2);
    assert(result.x == 4.0f && result.y == 10.0f && result.z == 18.0f);

    joj::vec3 v4 = joj::vec3_create(10.0f, 20.0f, 30.0f);
    joj::vec3 v5 = joj::vec3_create(40.0f, 50.0f, 60.0f);
    joj::vec3 result2 = joj::vec3_mul(v4, v5);
    assert(result2.x == 400.0f && result2.y == 1000.0f && result2.z == 1800.0f);
}

JOJ_TEST(vec3_div)
{
    joj::vec3 v1 = joj::vec3_create(6.0f, 9.0f, 12.0f);
    joj::vec3 v2 = joj::vec3_create(3.0f, 3.0f, 3.0f);
    joj::vec3 result = joj::vec3_div(v1, v2);
    assert(result.x == 2.0f && result.y == 3.0f && result.z == 4.0f);

    joj::vec3 v4 = joj::vec3_create(10.0f, 20.0f, 30.0f);
    joj::vec3 v5 = joj::vec3_create(2.0f, 5.0f, 10.0f);
    joj::vec3 result2 = joj::vec3_div(v4, v5);
    assert(result2.x == 5.0f && result2.y == 4.0f && result2.z == 3.0f);
}

JOJ_TEST(Length)
{
}

JOJ_TEST(Dot_Product)
{
}

JOJ_TEST(Cross_Product)
{
}

JOJ_TEST(Normalization)
{
}

int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}