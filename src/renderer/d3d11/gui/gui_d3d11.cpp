#include "renderer/d3d11/gui/gui_d3d11.h"

#if JPLATFORM_WINDOWS

#include "jmacros.h"

joj::D3D11GUI::D3D11GUI()
{
}

joj::D3D11GUI::~D3D11GUI()
{
}

void joj::D3D11GUI::init(GraphicsDevice& device)
{
    m_canvas = D3D11Canvas(600, 0, 200, 600, joj::Color(1.0f, 0.0f, 0.0f, 1.0f));
    JOJ_LOG_IF_FAIL(m_canvas.create(device));
}

void joj::D3D11GUI::update(const f32 dt)
{
}

void joj::D3D11GUI::draw(CommandList& cmd_list)
{
    m_canvas.draw(cmd_list);
}

void joj::D3D11GUI::shutdown()
{
}

#endif // JPLATFORM_WINDOWS