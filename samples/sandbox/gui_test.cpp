#include "gui_test.h"

#include "joj/engine/engine.h"

GUITest::GUITest()
{
}

GUITest::~GUITest()
{

}

void GUITest::init()
{
}

void GUITest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();
}

void GUITest::draw()
{
    m_renderer->clear(1.0f, 0.0f, 1.0f, 1.0f);
    m_renderer->swap_buffers();
}

void GUITest::shutdown()
{
}