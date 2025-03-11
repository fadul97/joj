#ifndef _JOJ_D3D11_GUI_H
#define _JOJ_D3D11_GUI_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "renderer/gui/gui.h"
#include <vector>
#include "gui/jwidget.h"
#include "gui/win32/jwidget_factory_win32.h"
#include "platform/win32/input_win32.h"
#include <memory>

namespace joj
{
    struct GUIWindow
    {
        HWND handle;
        HDC hdc;
        WindowMode window_mode;
        u32 x;
        u32 y;
        u32 width;
        u32 height;
    };


    class JOJ_API D3D11GUI : public IGUI
    {
    public:
        D3D11GUI();
        ~D3D11GUI();
        
        void init(WindowData& window, IRenderer& renderer) override;
        void update(const f32 dt, const i32 xmouse, const i32 ymouse,
            const b8 clicked) override;
        void draw(CommandList& cmd_list) override;
        void shutdown() override;

        void add_widget(JWidget* widget);
        void add_button(i32 x, i32 y, i32 width, i32 height,
            const std::string& label, const JGUIEvent::Callback& callback = nullptr) override;
        void add_button(const std::string& label, i32 x, i32 y, i32 width, i32 height,
            const std::function<void()>& function = nullptr);

    private:
        std::vector<JWidget*> m_widgets;
        // std::vector<std::unique_ptr<JWidget>> m_widgets2;
        b8 m_initialized;

        GUIWindow m_parent_window;
        GUIWindow m_window;
        JWin32WidgetFactory m_factory;

        static IRenderer* s_renderer;

        static LRESULT CALLBACK GUIWinProc(HWND hWnd, UINT msg, WPARAM wParam,
            LPARAM lParam);

        static Win32Input s_input;
        static Mouse s_mouse;
        static WNDPROC s_originalWndProc;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_GUI_H