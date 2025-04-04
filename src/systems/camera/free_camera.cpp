// FIXME: Remove DirectX dependency in this file.

#include "joj/systems/camera/free_camera.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/math/matrix4x4.h"
#include "joj/core/math/vector4.h"

joj::FreeCamera::FreeCamera()
    : m_movement_speed{20.0f}, m_view_dirty{true},
    m_position{0.0f, 0.0f, 0.0f}, m_right{1.0f, 0.0f, 0.0f},
    m_up{0.0f, 1.0f, 0.0f}, m_target{0.0f, 0.0f, 1.0f},
    m_nearz{0.0f}, m_farz{0.0f}, m_aspect{0.0f},
    m_yvof{0.0f}, m_near_window_height{0.0f},
    m_far_window_height{0.0f}
{
    m_view = Matrix4x4::identity();
    m_proj = Matrix4x4::identity();
    set_lens(0.25f * J_PI, 1.0f, 1.0f, 100.0f);

    update();
}

joj::FreeCamera::~FreeCamera()
{
}

joj::Vector3 joj::FreeCamera::get_pos() const
{
    return m_position;
}

void joj::FreeCamera::set_pos(const f32 x, const f32 y, const f32 z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;

    m_view_dirty = true;
    update();
}

void joj::FreeCamera::set_pos(const Vector3& v)
{
    m_position = v;
}

joj::Vector3 joj::FreeCamera::get_right() const
{
    return m_right;
}

joj::Vector3 joj::FreeCamera::get_up() const
{
    return m_up;
}

joj::Vector3 joj::FreeCamera::get_target() const
{
    return m_target;
}

f32 joj::FreeCamera::get_nearZ() const
{
    return m_nearz;
}

f32 joj::FreeCamera::get_farZ() const
{
    return m_farz;
}

f32 joj::FreeCamera::get_aspect() const
{
    return m_aspect;
}

f32 joj::FreeCamera::get_yfov() const
{
    return m_yvof;
}

f32 joj::FreeCamera::get_xfov() const
{
    f32 half_width = 0.5 * get_near_window_width();
    return 2.0f * atan(half_width / m_nearz);
}

f32 joj::FreeCamera::get_near_window_width() const
{
    return m_aspect * m_near_window_height;
}

f32 joj::FreeCamera::get_near_window_height() const
{
    return m_near_window_height;
}

f32 joj::FreeCamera::get_far_window_width() const
{
    return m_aspect * m_far_window_height;
}

f32 joj::FreeCamera::get_far_window_height() const
{
    return m_far_window_height;
}

void joj::FreeCamera::set_lens(const f32 yfov, const f32 aspect, const f32 znear, const f32 zfar)
{
    m_yvof = yfov;
    m_aspect = aspect;
    m_nearz = znear;
    m_farz = zfar;

    m_near_window_height = 2.0f * m_nearz * tanf(0.5f * m_yvof);
    m_far_window_height = 2.0f * m_farz * tanf(0.5f * m_yvof);

    JMatrix4x4 P = DirectX::XMMatrixPerspectiveFovLH(m_yvof, m_aspect, m_nearz, m_farz);

    m_proj.from_XMMATRIX(P);
}

void joj::FreeCamera::look_at(const Vector3& pos, const Vector3& target, const Vector3& world_up)
{
    Vector4 vpos = Vector4(pos);
    Vector4 vtarget = Vector4(target);
    Vector4 vup = Vector4(world_up);

    Vector4 L = vtarget - vpos;
    L.normalize();

    Vector4 R = vup.cross(L);
    R.normalize();

    Vector4 U = L.cross(R);

    m_position = Vector3(vpos.x, vpos.y, vpos.z);
    m_target = Vector3(L.x, L.y, L.z);
    m_right = Vector3(R.x, R.y, R.z);
    m_up = Vector3(U.x, U.y, U.z);

    m_view_dirty = true;
    update();
}

const joj::Matrix4x4& joj::FreeCamera::get_view()
{
    assert(!m_view_dirty);
    return m_view;
}

const joj::Matrix4x4& joj::FreeCamera::get_proj()
{
    return m_proj;
}

void joj::FreeCamera::strafe(const f32 d)
{
    Vector4 s = Vector4(d, d, d, d);
    Vector4 r = Vector4(m_right);
    Vector4 p = Vector4(m_position);

    Vector4 srp = (s * r) + p;

    m_position = Vector3(srp.x, srp.y, srp.z);

    m_view_dirty = true;
    update();
}

void joj::FreeCamera::walk(const f32 d)
{
    Vector4 s = Vector4(d, d, d, d);
    Vector4 l = Vector4(m_target);
    Vector4 p = Vector4(m_position);

    Vector4 slp = (s * l) + p;

    m_position = Vector3(slp.x, slp.y, slp.z);

    m_view_dirty = true;
    update();
}

void joj::FreeCamera::move(const CameraMovement direction, const f32 dt)
{
    const f32 velocity = m_movement_speed * dt;
    Vector4 p;
    Vector4 f;
    Vector4 r;
    switch (direction)
    {
    case joj::CameraMovement::FORWARD:
    {
        // Get the position and target vectors
        p = Vector4(m_position);
        f = Vector4(m_target);

        // Perform scalar multiplication and vector addition
        Vector4 scale_res = f * velocity;
        r = p + scale_res;

        // Store the result back in the position attribute
        m_position = Vector3(r.x, r.y, r.z);
    }
        break;
    case joj::CameraMovement::BACKWARD:
    {
        // Get the position and target vectors
        p = Vector4(m_position);
        f = Vector4(m_target);

        // Perform scalar multiplication and vector subtraction
        Vector4 scale_res = f * velocity;
        r = p - scale_res;

        // Store the result back the position attribute
        m_position = Vector3(r.x, r.y, r.z);
    }
        break;
    case joj::CameraMovement::LEFT:
    {
        // Get the position and right vectors
        p = Vector4(m_position);
        r = Vector4(m_right);

        // Perform scalar multiplication and vector addition
        Vector4 scale_res = r * velocity;
        r = p + scale_res;

        // Store the result back the position attribute
        m_position = Vector3(r.x, r.y, r.z);
    }
        break;
    case joj::CameraMovement::RIGHT:
    {
        // Get the position and right vectors
        p = Vector4(m_position);
        r = Vector4(m_right);

        // Perform scalar multiplication and vector subtraction
        Vector4 scale_res = r * velocity;
        r = p - scale_res;

        // Store the result back the position attribute
        m_position = Vector3(r.x, r.y, r.z);
    }
        break;
    default:
        break;
    }
}

void joj::FreeCamera::pitch(const f32 angle)
{
    assert(m_right.x != 0.0f || m_right.y != 0.0f || m_right.z != 0.0f);
    assert(m_up.x != 0.0f || m_up.y != 0.0f || m_up.z != 0.0f);
    assert(m_target.x != 0.0f || m_target.y != 0.0f || m_target.z != 0.0f);

    Vector4 vright = Vector4(m_right);
    vright.normalize();

    JMatrix4x4 R = DirectX::XMMatrixRotationAxis(vright.to_XMVECTOR(), angle);

    Vector4 vup = Vector4(m_up);
    Vector4 vtarget = Vector4(m_target);

    JXMVector4 up_normal = DirectX::XMVector3TransformNormal(vup.to_XMVECTOR(), R);
    JXMVector4 target_normal = DirectX::XMVector3TransformNormal(vtarget.to_XMVECTOR(), R);

    m_up = Vector3(up_normal.m128_f32[0], up_normal.m128_f32[1], up_normal.m128_f32[2]);
    m_target = Vector3(target_normal.m128_f32[0], target_normal.m128_f32[1], target_normal.m128_f32[2]);

    m_view_dirty = true;
    update();
}

void joj::FreeCamera::rotateY(const f32 angle)
{
    assert(m_right.x != 0.0f || m_right.y != 0.0f || m_right.z != 0.0f);
    assert(m_up.x != 0.0f || m_up.y != 0.0f || m_up.z != 0.0f);
    assert(m_target.x != 0.0f || m_target.y != 0.0f || m_target.z != 0.0f);

    JMatrix4x4 R = DirectX::XMMatrixRotationY(angle);

    Vector4 vright = Vector4(m_right);
    Vector4 vup = Vector4(m_up);
    Vector4 vtarget = Vector4(m_target);

    JXMVector4 right_normal = DirectX::XMVector3TransformNormal(vright.to_XMVECTOR(), R);
    JXMVector4 up_normal = DirectX::XMVector3TransformNormal(vup.to_XMVECTOR(), R);
    JXMVector4 target_normal = DirectX::XMVector3TransformNormal(vtarget.to_XMVECTOR(), R);

    m_right = Vector3(right_normal.m128_f32[0], right_normal.m128_f32[1], right_normal.m128_f32[2]);
    m_up = Vector3(up_normal.m128_f32[0], up_normal.m128_f32[1], up_normal.m128_f32[2]);
    m_target = Vector3(target_normal.m128_f32[0], target_normal.m128_f32[1], target_normal.m128_f32[2]);

    m_view_dirty = true;
    update();
}

void joj::FreeCamera::update()
{
    if (m_view_dirty)
    {
        Vector4 R = Vector4(m_right);
        Vector4 U = Vector4(m_up);
        Vector4 L = Vector4(m_target);
        Vector4 P = Vector4(m_position);

        L.normalize();

        Vector4 res = L.cross(U);
        R = res.normalized();

        // U, L already ortho-normal, so no need to normalize cross product.
        R = U.cross(L);

        // Fill in the view matrix entries.
        using namespace DirectX;
        f32 x = -XMVectorGetX(DirectX::XMVector3Dot(P.to_XMVECTOR(), R.to_XMVECTOR()));
        f32 y = -XMVectorGetX(DirectX::XMVector3Dot(P.to_XMVECTOR(), U.to_XMVECTOR()));
        f32 z = -XMVectorGetX(DirectX::XMVector3Dot(P.to_XMVECTOR(), L.to_XMVECTOR()));

        m_right = Vector3(R.x, R.y, R.z);
        m_up = Vector3(U.x, U.y, U.z);
        m_target = Vector3(L.x, L.y, L.z);

        m_view.m[0]  = m_right.x;
        m_view.m[4]  = m_right.y;
        m_view.m[8]  = m_right.z;
        m_view.m[12] = x;

        m_view.m[1]  = m_up.x;
        m_view.m[5]  = m_up.y;
        m_view.m[9]  = m_up.z;
        m_view.m[13] = y;

        m_view.m[2]  = m_target.x;
        m_view.m[6]  = m_target.y;
        m_view.m[10] = m_target.z;
        m_view.m[14] = z;

        m_view.m[3]  = 0.0f;
        m_view.m[7]  = 0.0f;
        m_view.m[11] = 0.0f;
        m_view.m[15] = 1.0f;

        m_view_dirty = false;
    }
}

#endif // JOJ_PLATFORM_WINDOWS