#include "systems/physics/rect.h"

joj::Rect::Rect()
    : Geometry(), m_left(0.0f), m_top(0.0f), m_right(0.0f), m_bottom(0.0f)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::Rect(const f32 x1, const f32 y1, const f32 x2, const f32 y2)
    : Geometry(), m_left(x1), m_top(y1), m_right(x2), m_bottom(y2)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::~Rect()
{
}