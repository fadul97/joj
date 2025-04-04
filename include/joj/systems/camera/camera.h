#ifndef _JOJ_CAMERA_H
#define _JOJ_CAMERA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/math/jmath.h"
#include "joj/core/math/matrix4x4.h"
#include "joj/core/math/vector3.h"

namespace joj
{
    class JOJ_API Camera
    {
    public:
        Camera();
        virtual ~Camera();

        const Matrix4x4& get_view() const;
        const Matrix4x4& get_proj() const;

        void set_viewport(const f32 left, const f32 right, const f32 bottom, const f32 top);
        void set_zoom(const f32 zoom);
        virtual void set_position(const Vector3& position) = 0;
        virtual void set_position(const f32 x, const f32 y, const f32 z) = 0;
        virtual void set_rotation(const Vector3& rotation) = 0;

        virtual Vector3 get_position() const = 0;

        virtual void translate(const f32 dx, const f32 dy, const f32 dz) = 0;
        virtual void move_to(const f32 x, const f32 y, const f32 z) = 0;
        virtual void move(const Vector3& offset) = 0;

    protected:
        Matrix4x4 m_view;
        Matrix4x4 m_proj;

        b8 m_view_dirty;

        f32 m_left;
        f32 m_right;
        f32 m_bottom;
        f32 m_top;

        f32 m_zoom;
        f32 m_rotation;

        f32 m_x;
        f32 m_y;

        virtual void update() = 0;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_CAMERA_H