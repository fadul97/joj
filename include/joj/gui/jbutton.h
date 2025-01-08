#ifndef _JOJ_JBUTTON_H
#define _JOJ_JBUTTON_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "jwidget.h"
#include "platform/window.h"
#include <string>

namespace joj
{
    class JAPI JButton : public JWidget
    {
    public:
        JButton();
        JButton(const u32 x, const u32 y, const u32 width, const u32 height,
            const char* title);
        ~JButton();

        void create(JWidgetCreationData& data) override;
        void draw(CommandList& cmd_list) override;
        void update(i32 xmouse, i32 ymouse, b8 clicked) override;

        b8 is_hovered(const i32 x, const i32 y) override;

        void on_click(const JEvent::Callback& callback) override;

        LRESULT handle_message(UINT msg, WPARAM wParam, LPARAM lParam) override;

    private:
        WidgetHandle m_handle;
        WindowRect m_bounds;
        std::string m_label;
    };
}

#endif // _JOJ_JBUTTON_H