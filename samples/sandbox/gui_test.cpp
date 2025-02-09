#include "gui_test.h"

#include "joj/core/logger.h"
#include <sstream>
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"

GUITest::GUITest()
{
}

GUITest::~GUITest()
{

}

f32 red = 0.0f;
void on_button_clicked() {
    red += 0.20f;
    JINFO("Button clicked!")
}

void GUITest::init()
{

    // Create a non-const copy of window data to pass to m_gui.init
    joj::WindowData windowData = m_window->get_data();
    m_gui.init(windowData, *m_renderer);

    // Create a button
    m_gui.add_button("Click Me!", 10, 10, 100, 30, on_button_clicked);
}

void GUITest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    m_gui.update(dt, m_input->get_xmouse(), m_input->get_ymouse(),
        m_input->is_button_down(joj::BUTTON_LEFT));
}

void GUITest::draw()
{
    m_renderer->clear(red, 0.0f, 1.0f, 1.0f);

    // renderer.disable_depth_test();
    m_gui.draw(m_renderer->get_cmd_list());
    // renderer.enable_depth_test();

    m_renderer->swap_buffers();
}

void GUITest::shutdown()
{
    m_gui.shutdown();
}