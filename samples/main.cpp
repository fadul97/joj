#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include <sstream>
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include "logger.h"

joj::Win32Window window;
joj::Win32Input input;
joj::Win32Timer timer;
joj::D3D11Renderer renderer;
b8 loop = true;
f32 frametime = 0.0f;
f32 get_frametime();
b8 initialize_everything();
void shutdown_everything();
b8 process_events();
void update();
void draw();

int main()
{
    if (!initialize_everything())
        return 1;

    while (loop)
    {
        if (!process_events())
            loop = false;

        frametime = get_frametime();

        update();

        draw();
    }

    shutdown_everything();

    return 0;
}

f32 get_frametime()
{
#ifdef JOJ_DEBUG_MODE
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter
#endif // JOJ_DEBUG_MODE

    // Current frame time
    frametime = timer.reset();

#ifdef JOJ_DEBUG_MODE
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

        text << "Joj Engine v0.0.1" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << frametime * 1000 << " (ms)";

        window.set_title(text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
#endif // JOJ_DEBUG_MODE

    return frametime;
}

b8 initialize_everything()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    if (window.create() != joj::ErrorCode::OK)
        return false;

    u32 width = 0;
    u32 height = 0;

    window.get_window_size(width, height);
    JDEBUG("Window size: %dx%d", width, height);

    window.get_client_size(width, height);
    JDEBUG("Client size: %dx%d", width, height);

    input.set_window(window.get_data());

    timer.begin_period();

    renderer_print();

    if (renderer.initialize(window.get_data()) != joj::ErrorCode::OK)
        return false;

    timer.start();

    return true;
}

void shutdown_everything()
{
    timer.end_period();
}

b8 process_events()
{
    MSG msg = { 0 };
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            return false;
    }

    return true;
}

void update()
{
    if (input.is_key_pressed('A'))
        JDEBUG("A pressed.");

    if (input.is_key_down(joj::KEY_SPACE))
        JDEBUG("Space down.");

    if (input.is_key_pressed(joj::KEY_ESCAPE))
        loop = false;
}

void draw()
{
    renderer.clear(1.0f, 1.0f, 0.0f, 1.0f);
    renderer.swap_buffers();
}
