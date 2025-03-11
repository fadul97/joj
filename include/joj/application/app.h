#ifndef JOJ_APP_H
#define JOJ_APP_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <Windows.h>
#include "platform/keys.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "platform/win32/timer_win32.h"
#include "renderer/d3d11/renderer_d3d11.h"

namespace joj
{
    class JOJ_API App
    {
    public:
        App();
        virtual ~App();

        virtual void init() = 0;
        virtual void update(const f32 dt) = 0;
        virtual void shutdown() = 0;

        virtual void on_mouse_down(Buttons button, i32 x, i32 y) {}
        virtual void on_mouse_up(Buttons button, i32 x, i32 y) {}
        virtual void on_mouse_move(WPARAM button_state, i32 x, i32 y) {}

        /* These methods have an empty implementation by default
         * and only one of them must be overridden in the derived class:
         * - Draw is executed every cycle of the main loop.
         * - Display is called only once at the start of the application
         * and must be called manually in Update every time the screen needs
         * to be redrawn.
         */
        virtual void draw() {}
        virtual void display() {}

        virtual void on_pause() { Sleep(10); }

        void set_window(Win32Window* window);
        void set_input(Win32Input* input);
        void set_timer(Win32Timer* timer);
        void set_renderer(D3D11Renderer* renderer);

    protected:
        Win32Window* m_window;
        Win32Input* m_input;
        Win32Timer* m_timer;
        D3D11Renderer* m_renderer;
    };

    inline void App::set_window(Win32Window* window)
    { m_window = window; }

    inline void App::set_input(Win32Input* input)
    { m_input = input; }

    inline void App::set_timer(Win32Timer* timer)
    { m_timer = timer; }

    inline void App::set_renderer(D3D11Renderer* renderer)
    { m_renderer = renderer; }
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // JOJ_APP_H