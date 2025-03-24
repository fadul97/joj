#ifndef _JOJ_JBUTTON_H
#define _JOJ_JBUTTON_H

#include "joj/core/defines.h"

#include "jwidget.h"
#include "joj/platform/window.h"
#include <string>
#include <Windows.h>
#include "joj/core/logger.h"

namespace joj
{
    class JOJ_API JButton : public JWidget
    {
    public:
        JButton();
        JButton(const u32 x, const u32 y, const u32 width, const u32 height,
            const char* title);
        ~JButton();

        void create(JWidgetCreationData& data, const JGUIEvent::Callback& callback) override;
        void draw(CommandList& cmd_list) override;
        void update(i32 xmouse, i32 ymouse, b8 clicked) override;

        b8 is_hovered(const i32 x, const i32 y) override;

        void on_click(const JGUIEvent::Callback& callback) override;
        void trigger() override;

        void set_callback(const std::function<void()>& callback);

        LRESULT handle_message(UINT msg, WPARAM wParam, LPARAM lParam) override;

        static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT msg, WPARAM wParam,
            LPARAM lParam);
    
    public:
        WidgetHandle m_handle;
        WindowRect m_bounds;
        std::string m_label;
        std::function<void()> m_callback;
    };

    inline void JButton::set_callback(const std::function<void()>& callback)
    {
        if (callback)
            m_callback = callback;
    }
}

#endif // _JOJ_JBUTTON_H