#include "systems/physics/rect.h"

joj::Rect::Rect()
    : m_left(0.0f), m_top(0.0f), m_right(0.0f), m_bottom(0.0f),
    m_width(0.0f), m_height(0.0f)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::Rect(const f32 width, const f32 height)
    : m_left(-width / 2.0f),
    m_top(-height / 2.0f),
    m_right(width / 2.0f),
    m_bottom(height / 2.0f),
    m_width(width), m_height(height)
{
    m_type = GeometryType::Rectangle;
}

joj::Rect::~Rect()
{
}

b8 joj::Rect::check_collision(const Geometry& geometry)
{
    switch (geometry.get_type())
    {
    case GeometryType::Rectangle:
    {
        auto rect = static_cast<const Rect*>(&geometry);
        m_colliding = get_right() >= rect->get_left() &&
            get_left() <= rect->get_right() &&
            get_bottom() >= rect->get_top() &&
            get_top() <= rect->get_bottom();
        return m_colliding;
    }
    case GeometryType::Unknown:
        return false;
    default:
        return false;
    }
}