#ifndef _JOJ_JWIDGET_H
#define _JOJ_JWIDGET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <Windows.h>
#include "jevent.h"

namespace joj
{
    struct JWidgetCreationData
    {
        HWND parent_handle;
        HINSTANCE instance;
    };

    struct CommandList;

    struct WidgetHandle
    {
        HWND handle;
    };

    class JAPI JWidget
    {
    public:
        JWidget();
        JWidget(const u32 x, const u32 y, const u32 width, const u32 height,
            const char* title);
        virtual ~JWidget() = default;

        virtual void create(JWidgetCreationData& data) = 0;
        virtual void draw(CommandList& cmd_list) = 0;
        virtual void update(i32 xmouse, i32 ymouse, b8 clicked) = 0;

        virtual b8 is_hovered(const i32 x, const i32 y) = 0;

        virtual void on_click(const JEvent::Callback& callback) = 0;

    protected:
        u32 m_x;
        u32 m_y;
        u32 m_width;
        u32 m_height;

        JEvent m_on_click;
        b8 m_is_hovered;
    };
}

#endif // _JOJ_JWIDGET_H