#include "joj/systems/physics/geometry.h"

joj::Geometry::Geometry()
    : m_x(0.0f), m_y(0.0f), m_type(GeometryType::Unknown),
    m_colliding(false), m_rotation(0.0f)
{
}

joj::Geometry::~Geometry()
{
}