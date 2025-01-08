#ifndef _JOJ_GUI_H
#define _JOJ_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
#include "platform/window.h"
#include "platform/input.h"
#include "gui/jwidget.h"
#include <string>

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
        virtual void add_button(i32 x, i32 y, i32 width, i32 height,
            const std::string& label, const JEvent::Callback& callback = nullptr) = 0;
    };
}

#endif // _JOJ_GUI_H