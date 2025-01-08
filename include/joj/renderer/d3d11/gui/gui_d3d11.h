#ifndef _JOJ_D3D11_GUI_H
#define _JOJ_D3D11_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/gui/gui.h"
#include <vector>
#include "gui/jwidget.h"

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


    class JAPI D3D11GUI : public IGUI
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

    private:
        std::vector<JWidget*> m_widgets;
        b8 m_initialized;

        GUIWindow m_main_window;

        static IRenderer* s_renderer;

        static LRESULT CALLBACK GUIWinProc(HWND hWnd, UINT msg, WPARAM wParam,
            LPARAM lParam);
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_GUI_H