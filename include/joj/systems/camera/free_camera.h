#ifndef _JOJ_FREE_CAMERA_H
#define _JOJ_FREE_CAMERA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/math/jmath.h"
#include "joj/core/math/matrix4x4.h"
#include "joj/core/math/vector3.h"

namespace joj
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const f32 YAW = -90.0f;
    const f32 PITCH = 0.0f;
    const f32 SPEED = 10.0f;
    const f32 SENSITIVITY = 0.1f;
    const f32 ZOOM = 45.0f;

    class JOJ_API FreeCamera
    {
    public:
        FreeCamera();
        ~FreeCamera();

        Vector3 get_pos() const;
        void set_pos(const f32 x, const f32 y, const f32 z);
        void set_pos(const Vector3& v);

        Vector3 get_right() const;
        Vector3 get_up() const;
        Vector3 get_target() const;

        f32 get_nearZ() const;
        f32 get_farZ() const;
        f32 get_aspect() const;
        f32 get_yfov() const;
        f32 get_xfov() const;

        f32 get_near_window_width() const;
        f32 get_near_window_height() const;
        f32 get_far_window_width() const;
        f32 get_far_window_height() const;

        void set_lens(const f32 yfov, const f32 aspect, const f32 znear, const f32 zfar);

        void look_at(const Vector3& pos, const Vector3& target, const Vector3& world_up);

        const Matrix4x4& get_view();
        const Matrix4x4& get_proj();

        void strafe(const f32 d);
        void walk(const f32 d);

        // FIXME: 
        void move(const CameraMovement direction, const f32 dt);

        void pitch(const f32 angle);
        void rotateY(const f32 angle);

    private:
        Vector3 m_position;
        Vector3 m_right;
        Vector3 m_up;
        Vector3 m_target;

        f32 m_nearz;
        f32 m_farz;
        f32 m_aspect;
        f32 m_yvof;
        f32 m_near_window_height;
        f32 m_far_window_height;

        b8 m_view_dirty;

        Matrix4x4 m_view;
        Matrix4x4 m_proj;

        f32 m_movement_speed;

        void update();
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_FREE_CAMERA_H