#ifndef _JOJ_RECT_H
#define _JOJ_RECT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "geometry.h"

namespace joj
{
    class JAPI Rect : public Geometry
    {
    public:
        Rect();
        Rect(const f32 width, const f32 height);
        ~Rect();

        b8 check_collision(const Geometry& geometry) override;

        void set_width(const f32 width);
        void set_height(const f32 height);
        void set_size(const f32 width, const f32 height);
        void set_size(const JFloat2& size);

        f32 get_rotation() override;

        f32 get_width() const;
        f32 get_height() const;

        JFloat2 get_size() const;

        f32 get_left() const;
        f32 get_top() const;
        f32 get_right() const;
        f32 get_bottom() const;

    private:
        f32 m_left;
        f32 m_top;
        f32 m_right;
        f32 m_bottom;

        f32 m_width;
        f32 m_height;
    };

    inline void Rect::set_width(const f32 width)
    {
        m_width = width;
        m_left = -width / 2.0f;
        m_right = width / 2.0f;
    }

    inline void Rect::set_height(const f32 height)
    {
        m_height = height;
        m_top = -height / 2.0f;
        m_bottom = height / 2.0f;
    }

    inline void Rect::set_size(const f32 width, const f32 height)
    { set_width(width); set_height(height); }

    inline void Rect::set_size(const JFloat2& size)
    { set_size(size.x, size.y); }

    inline f32 Rect::get_width() const
    { return m_width; }

    inline f32 Rect::get_height() const
    { return m_height; }

    inline JFloat2 Rect::get_size() const
    { return { m_width, m_height }; }

    inline f32 Rect::get_left() const
    { return m_x + m_left; }

    inline f32 Rect::get_top() const
    { return m_y + m_top; }

    inline f32 Rect::get_right() const
    { return m_x + m_right; }

    inline f32 Rect::get_bottom() const
    { return m_y + m_bottom; }
}

#endif // _JOJ_RECT_H