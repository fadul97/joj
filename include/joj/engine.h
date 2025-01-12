/**
 * @file engine.h
 * @author Leonardo Fadul ( TODO: todo@domain.com)
 * @brief This file WILL contain the engine specific code.
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright TODO: Copyright (c) 2024
 * 
 * REFERENCES: Travis Vroman (https://travisvroman.com/) - Kohi Game Engine (https://github.com/travisvroman/kohi)
 * 
 */

#ifndef _JOJ_ENGINE_H
#define _JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <memory>
#include "application/app.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "platform/win32/timer_win32.h"
#include "renderer/d3d11/renderer_d3d11.h"
#include "error_code.h"

JAPI void joj_print();

namespace joj
{
    class JAPI Engine
    {
    public:
        Engine();
        ~Engine();

        void init();
        void update(const f32 dt);
        void draw();
        void shutdown(); 

        ErrorCode start();
        i32 run(App* app);

        static void on_resize();

        static void close();

        static void pause();	// Pause engine
        static void resume();	// Resume engine

        // Handle Windows events
        static LRESULT CALLBACK EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        static Win32Window* s_window;
        static Win32Input* s_input;
        static Win32Timer* s_timer;
        static D3D11Renderer* s_renderer;
        static App* s_app;

        static b8 s_paused;
        static b8 s_running;
        static f32 s_frametime;

        static b8 s_resizing;
        static b8 s_minimized;
        static b8 s_maximized;
        static u32 s_client_width;
        static u32 s_client_height;

        f32 get_frametime();
    };

    inline void Engine::pause()
    { s_paused = true; s_timer->stop(); }

    inline void Engine::resume()
    { s_paused = false; s_timer->start(); }

    inline void Engine::close()
    { s_running = false; }
}

#endif // _JOJ_ENGINE_H
