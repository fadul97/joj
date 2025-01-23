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

        f32 get_left() const;
        f32 get_top() const;
        f32 get_right() const;
        f32 get_bottom() const;

    private:
        f32 m_left;
        f32 m_top;
        f32 m_right;
        f32 m_bottom;
    };

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