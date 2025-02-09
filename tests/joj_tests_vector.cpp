#include "joj/test/jtest_macros.h"
#include "core/jvector.h"

#include <iostream>


JOJ_TEST(Constructor_Default_Size)
{
    joj::Vector<i32> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    assert(vec.get_size() == 3);
    assert(vec.get_capacity() == 4);
    assert(vec.get_memory_usage() == sizeof(i32) * vec.get_capacity());
}

JOJ_TEST(Constructor_Default_Capacity)
{
    joj::Vector<f32> vec;

    vec.push_back(10.0f);
    vec.push_back(20.0f);
    vec.push_back(30.0f);

    assert(vec.get_size() == 3);
    assert(vec.get_capacity() == 4);
    assert(vec.get_memory_usage() == sizeof(f32) * vec.get_capacity());
}

JOJ_TEST(Constructor_Size_Param)
{
    joj::Vector<u16> vec(5);
    assert(vec.get_size() == 5);
    assert(vec.get_capacity() == 5);
    assert(vec.get_memory_usage() == sizeof(u16) * vec.get_capacity());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    assert(vec.get_size() == 8);
    assert(vec.get_capacity() == 10);
    assert(vec.get_memory_usage() == sizeof(u16) * vec.get_capacity());
}

JOJ_TEST(Constructor_Size_DefaultValue_Params)
{
    joj::Vector<b8> vec(5, true);

    vec.push_back(true);
    vec.push_back(false);
    vec.push_back(true);
    vec.push_back(false);
    vec.push_back(false);

    assert(vec.get_size() == 10);
    assert(vec.get_capacity() == 10);
}

JOJ_TEST(OperatorBrackets)
{
    joj::Vector<char> vec(5, 'A');

    vec.push_back('B');
    vec.push_back('C');
    vec.push_back('D');
    vec.push_back('E');
    vec.push_back('F');

    assert(vec.get_size() == 10);
    assert(vec.get_capacity() == 10);
    assert(vec[6] == 'C');
}

JOJ_TEST(Copy_Constructor)
{
    joj::Vector<i32> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    joj::Vector<i32> vec2 = vec1; // Testando o construtor de cópia
    assert(vec2.get_size() == 3);
    assert(vec2.get_capacity() >= 3);
    assert(vec2[0] == 1 && vec2[1] == 2 && vec2[2] == 3);
}

JOJ_TEST(Move_Constructor)
{
    joj::Vector<i32> vec1;
    vec1.push_back(42);
    vec1.push_back(84);

    joj::Vector<i32> vec2 = std::move(vec1); // Testando o construtor de movimentação
    assert(vec2.get_size() == 2);
    assert(vec2.get_capacity() >= 2);
    assert(vec2[0] == 42 && vec2[1] == 84);
}

JOJ_TEST(PopBack)
{
    joj::Vector<i32> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    vec.pop_back();
    assert(vec.get_size() == 2);
    assert(vec[0] == 10 && vec[1] == 20);
}

JOJ_TEST(Clear)
{
    joj::Vector<i32> vec;
    vec.push_back(5);
    vec.push_back(10);
    vec.push_back(15);

    vec.clear();
    assert(vec.get_size() == 0);
    assert(vec.get_capacity() > 0); // Capacidade pode ser mantida
}


int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}