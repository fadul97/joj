#ifndef _JOJ_JWIDGET_H
#define _JOJ_JWIDGET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <Windows.h>

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

    private:
        u32 m_x;
        u32 m_y;
        u32 m_width;
        u32 m_height;
    };
}

#endif // _JOJ_JWIDGET_H