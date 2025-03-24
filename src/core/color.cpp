#include "joj/core/color.h"

const joj::Color joj::Color::White = joj::Color(1.0f, 1.0f, 1.0f);
const joj::Color joj::Color::Black = joj::Color(0.0f, 0.0f, 0.0f);
const joj::Color joj::Color::Red = joj::Color(1.0f, 0.0f, 0.0f);
const joj::Color joj::Color::Green = joj::Color(0.0f, 1.0f, 0.0f);
const joj::Color joj::Color::Blue = joj::Color(0.0f, 0.0f, 1.0f);
const joj::Color joj::Color::Yellow = joj::Color(1.0f, 1.0f, 0.0f);

joj::Color::Color()
    : m_r(1.0f), m_g(0.0f), m_b(1.0f), m_a(1.0f)
{
}

joj::Color::Color(const f32 r, const f32 g, const f32 b, const f32 a)
    : m_r(r), m_g(g), m_b(b), m_a(a)
{
}

joj::IntColor joj::Color::to_IntColor() const
{
    return IntColor(
        static_cast<u8>(m_r * 255.0f),
        static_cast<u8>(m_g * 255.0f),
        static_cast<u8>(m_b * 255.0f),
        static_cast<u8>(m_a * 255.0f)
    );
}

joj::FloatColor joj::Color::to_FloatColor() const
{
    return FloatColor(m_r, m_g, m_b, m_a);
}

f32 joj::Color::get_r() const
{
    return m_r;
}

f32 joj::Color::get_g() const
{
    return m_g;
}

f32 joj::Color::get_b() const
{
    return m_b;
}

f32 joj::Color::get_a() const
{
    return m_a;
}