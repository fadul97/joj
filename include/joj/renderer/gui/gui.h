#ifndef _JOJ_GUI_H
#define _JOJ_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
#include "platform/window.h"
#include "gui/jwidget.h"

namespace joj
{
    class JAPI IGUI
    {
    public:
        IGUI();
        virtual ~IGUI();

        virtual void init(WindowData& window, IRenderer& renderer) = 0;
        virtual void update(const f32 dt, const i32 xmouse, const i32 ymouse,
            const b8 clicked) = 0;
        virtual void draw(CommandList& cmd_list) = 0;
        virtual void shutdown() = 0;

        virtual void add_widget(JWidget* widget) = 0;
    };
}

#endif // _JOJ_GUI_H