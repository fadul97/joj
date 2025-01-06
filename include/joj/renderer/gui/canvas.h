#ifndef _JOJ_CANVAS_H
#define _JOJ_CANVAS_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/color.h"
#include "error_code.h"
#include "renderer/renderer.h"

namespace joj
{
    class JAPI Canvas
    {
    public:
        Canvas();
        Canvas(const u16 x, const u16 y, const u16 width, const u16 height);
        Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color);
        ~Canvas();

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void draw(CommandList& cmd_list) = 0;

    protected:
        u16 m_x;
        u16 m_y;
        u16 m_width;
        u16 m_height;
        Color m_background_color;

        f32 m_normalized_x;
        f32 m_normalized_y;
        f32 m_normalized_width;
        f32 m_normalized_height;
    };
}
#endif // _JOJ_CANVAS_H