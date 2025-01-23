#include "systems/physics/geometry.h"

joj::Geometry::Geometry()
    : m_x(0.0f), m_y(0.0f), m_type(GeometryType::Unknown),
    m_colliding(false)
{
}

joj::Geometry::~Geometry()
{
}