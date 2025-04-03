#include "joj/systems/camera/camera2d.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/logger.h"

joj::Camera2D::Camera2D()
    : Camera()
{
    update();
}

joj::Camera2D::Camera2D(const f32 left, const f32 right, const f32 bottom, const f32 top)
    : Camera()
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    update();
}

joj::Camera2D::~Camera2D()
{
}

void joj::Camera2D::set_position(const JFloat3& position)
{
    m_x = position.x;
    m_y = position.y;
    m_view_dirty = true;
    update();
}

void joj::Camera2D::set_position(const f32 x, const f32 y, const f32 z)
{
    m_x = x;
    m_y = y;
    m_view_dirty = true;
    update();
}

void joj::Camera2D::set_rotation(const JFloat3& rotation)
{
    JOJ_TODO();
}

joj::JFloat3 joj::Camera2D::get_position() const
{
    return JFloat3{ m_x, m_y, 0.0f };
}

void joj::Camera2D::translate(const f32 dx, const f32 dy, const f32 dz)
{
    m_x += dx;
    m_y += dy;
    m_view_dirty = true;
    update();
}

void joj::Camera2D::move_to(const f32 x, const f32 y, const f32 z)
{
    m_x = x;
    m_y = y;
    m_view_dirty = true;
    update();
}

void joj::Camera2D::move(const JFloat3& offset)
{
    m_x += offset.x;
    m_y += offset.y;
    m_view_dirty = true;
    update();
}

void joj::Camera2D::update()
{
    if (m_view_dirty)
    {
        // Matriz de vis�o: rota��o + transla��o
        JMatrix4x4 translation = DirectX::XMMatrixTranslation(-m_x, -m_y, 0.0f);
        JMatrix4x4 rotation = DirectX::XMMatrixRotationZ(m_rotation);
        JMatrix4x4 view = rotation * translation;

        // Matriz de proje��o: ortogr�fica
        JMatrix4x4 proj = DirectX::XMMatrixOrthographicOffCenterLH(
            m_left / m_zoom, m_right / m_zoom, m_bottom / m_zoom, m_top / m_zoom, -1.0f, 1.0f);

        // Armazena as matrizes
        DirectX::XMStoreFloat4x4(&m_view, view);
        DirectX::XMStoreFloat4x4(&m_proj, proj);

        m_view_dirty = false;
    }
}

#endif // JOJ_PLATFORM_WINDOWS