#include "gui_test.h"

#include "logger.h"
#include <sstream>
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"

GUITest::GUITest()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    input = joj::Win32Input();
    timer = joj::Win32Timer();
    renderer = joj::D3D11Renderer();
}

GUITest::~GUITest()
{

}

void GUITest::init_platform()
{
    if (window.create() != joj::ErrorCode::OK)
        return;

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
        return;

    timer.start();
}

void GUITest::init()
{
    init_platform();

    m_gui.init(renderer.get_device());
}

void GUITest::update(const f32 dt)
{
    if (input.is_key_pressed(joj::KEY_ESCAPE))
        loop = false;

    m_gui.update(dt);
}

void GUITest::draw()
{
    renderer.clear(0.0f, 0.0f, 1.0f, 1.0f);

    m_gui.draw(renderer.get_cmd_list());

    renderer.swap_buffers();
}

void GUITest::shutdown()
{
    timer.end_period();
}

f32 GUITest::get_frametime()
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