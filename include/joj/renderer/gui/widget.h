#ifndef _JOJ_WIDGET_H
#define _JOJ_WIDGET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/color.h"
#include "error_code.h"
#include "renderer/renderer.h"

namespace joj
{
    class JAPI Widget
    {
    public:
        Widget();
        Widget(const u16 x, const u16 y, const u16 width, const u16 height);
        Widget(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        virtual ~Widget();

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void draw(CommandList& cmd_list) = 0;

        virtual b8 is_hovered(const i32 x, const i32 y) = 0;

        virtual void update(const i32 xmouse, const i32 ymouse, const b8 clicked) = 0;
        virtual void should_update() = 0;

        virtual void set_background_color(const Color color) = 0;
        virtual void set_hovered_color(const Color color) = 0;

        virtual b8 on_left_edge(const i32 x, const i32 y) = 0;
        virtual b8 on_right_edge(const i32 x, const i32 y) = 0;
        virtual b8 on_top_edge(const i32 x, const i32 y) = 0;
        virtual b8 on_bottom_edge(const i32 x, const i32 y) = 0;

    protected:
        u16 m_x;
        u16 m_y;
        u16 m_width;
        u16 m_height;
        Color m_background_color;
        Color m_hovered_color;

        f32 m_normalized_x;
        f32 m_normalized_y;
        f32 m_normalized_width;
        f32 m_normalized_height;

        b8 m_update;
        b8 m_update_position;
    };
}

#endif // _JOJ_WIDGET_H