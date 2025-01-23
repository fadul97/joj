#include "systems/physics/rect.h"

joj::Rect::Rect()
    : m_left(0.0f), m_top(0.0f), m_right(0.0f), m_bottom(0.0f)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::Rect(const f32 width, const f32 height)
    : m_left(-width / 2.0f),
    m_top(-height / 2.0f),
    m_right(width / 2.0f),
    m_bottom(height / 2.0f)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::~Rect()
{
}