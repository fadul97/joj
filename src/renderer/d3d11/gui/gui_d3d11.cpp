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
    D3D11Canvas* m_canvas = new D3D11Canvas(600, 0, 200, 600, joj::Color(0.1f, 0.1f, 0.1f, 1.0f));
    JOJ_LOG_IF_FAIL(m_canvas->create(device));
    m_canvas->set_hovered_color(joj::Color(0.23f, 0.23f, 0.23f, 1.0f));

    D3D11Button* m_button = new D3D11Button(600, 0, 100, 100, joj::Color(0.0f, 1.0f, 0.0f, 1.0f));
    JOJ_LOG_IF_FAIL(m_button->create(device));
    m_button->set_hovered_color(joj::Color(1.0f, 0.0f, 0.0f, 1.0f));

    m_widgets.push_back(m_canvas);
    m_widgets.push_back(m_button);
}

void joj::D3D11GUI::update(const f32 dt, const i32 xmouse, const i32 ymouse)
{
    if (xmouse < 0 || ymouse < 0)
        return;

    const f32 window_width = 800.0f;
    const f32 window_height = 600.0f;

    f32 ndc_x = 2.0f * static_cast<f32>(xmouse) / window_width - 1.0f;
    f32 ndc_y = 1.0f - 2.0f * static_cast<f32>(ymouse) / window_height;

    i32 i = -1;
    for (auto& widget : m_widgets)
    {
        if (widget->is_hovered(xmouse, ymouse))
        {
            ++i;
        }
    }

    if (i >= 0 && i < m_widgets.size())
    {
        m_widgets[i]->update();
    }
}

void joj::D3D11GUI::draw(CommandList& cmd_list)
{
    for (auto& widget : m_widgets)
    {
        widget->draw(cmd_list);
    }
}

void joj::D3D11GUI::shutdown()
{
}

#endif // JPLATFORM_WINDOWS