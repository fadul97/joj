#include "systems/camera/camera.h"

joj::Camera::Camera()
    : m_view{ float4x4_identity() }, m_proj{ float4x4_identity() },
    m_left{ -1.0f }, m_right{ 1.0f },
    m_bottom{ 1.0f }, m_top{ -1.0f },
    m_zoom{ 1.0f }, m_rotation{ 0.0f },
    m_x{ 0.0f }, m_y{ 0.0f },
    m_view_dirty{ true }
{
}

joj::Camera::~Camera()
{
}

const joj::JFloat4x4& joj::Camera::get_view() const
{
    return m_view;
}

const joj::JFloat4x4& joj::Camera::get_proj() const
{
    return m_proj;
}

void joj::Camera::set_viewport(const f32 left, const f32 right, const f32 bottom, const f32 top)
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    update();
}

void joj::Camera::set_zoom(const f32 zoom)
{
    m_zoom = zoom;
    update();
}