#include <iostream>
#include "joj/logger.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include <sstream>
#include "joj/renderer/d3d11/renderer_d3d11.h"

joj::Win32Window window;
joj::Win32Input input;
joj::Win32Timer timer;
f32 frametime = 0.0f;
f32 get_frametime();

int main()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    if (window.create() != joj::ErrorCode::OK)
    {
        JFATAL(joj::ErrorCode::FAILED, "Failed to create Win32Window");
        return -1;
    }

    u32 width = 0;
    u32 height = 0;

    window.get_window_size(width, height);
    JDEBUG("Window size: %dx%d", width, height);

    window.get_client_size(width, height);
    JDEBUG("Client size: %dx%d", width, height);

    input.set_window(window.get_data());

    timer.begin_period();

    MSG msg = { 0 };
    bool loop = true;

    renderer_print();

    joj::D3D11Renderer renderer;
    if (renderer.initialize(window.get_data()) != joj::ErrorCode::OK)
    {
        JFATAL(joj::ErrorCode::FAILED, "Failed to create D3D11 Context");
        return -2;
    }

    timer.start();
    while (loop)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
                loop = false;
        }

        frametime = get_frametime();

        if (input.is_key_pressed('A'))
            JDEBUG("A pressed.");

        if (input.is_key_down(joj::KEY_SPACE))
            JDEBUG("Space down.");

        if (input.is_key_pressed(joj::KEY_ESCAPE))
            loop = false;

        renderer.clear(1.0f, 1.0f, 0.0f, 1.0f);
        renderer.swap_buffers();
    }

    timer.end_period();

    return 0;
}

f32 get_frametime()
{
#ifdef _DEBUG
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter
#endif

    // Current frame time
    frametime = timer.reset();

#ifdef _DEBUG
    // Accumulated frametime
    total_time += frametime;

    // Increment frame counter
    frame_count++;

    // Updates FPS indicator in the window every 1000ms (1 second)
    if (total_time >= 1.0f)
    {
        std::stringstream text;		// Text flow for messages
        text << std::fixed;			// Always show the fractional part
        text.precision(3);			// three numbers after comma

        text << "Joj Engine v0.0.2" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << frametime * 1000 << " (ms)";

        SetWindowText(window.get_data().handle, text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
#endif

    return frametime;
}
