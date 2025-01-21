#ifndef _JOJ_GEOMETRY_H
#define _JOJ_GEOMETRY_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"

namespace joj
{
    enum class GeometryType
    {
        Unknown,
        Rectangle,
    };

    class JAPI Geometry
    {
    public:
        Geometry();
        virtual ~Geometry();

        virtual void set_position(const f32 x, const f32 y);
        virtual void set_position(const JFloat2& position);
        virtual void set_type(const GeometryType type);

        virtual JFloat2 get_position2D() const;
        virtual GeometryType get_type() const;

        virtual void translate(const f32 dx, const f32 dy);
        virtual void move_to(const f32 x, const f32 y);

    protected:
        f32 m_x;
        f32 m_y;
        GeometryType m_type;
    };

    inline void Geometry::set_position(const f32 x, const f32 y)
    { m_x = x; m_y = y; }

    inline void Geometry::set_position(const JFloat2& position)
    { m_x = position.x; m_y = position.y; }

    inline void Geometry::set_type(const GeometryType type)
    { m_type = type; }

    inline JFloat2 Geometry::get_position2D() const
    { return { m_x, m_y }; }

    inline GeometryType Geometry::get_type() const
    { return m_type; }

    inline void Geometry::translate(const f32 dx, const f32 dy)
    { m_x += dx; m_y += dy; }

    inline void Geometry::move_to(const f32 x, const f32 y)
    { m_x = x; m_y = y; }
}

#endif // _JOJ_GEOMETRY_H