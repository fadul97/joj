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
    D3D11Canvas* m_canvas = new D3D11Canvas(
        580, 10, 200, 580,
        joj::Color(0.1f, 0.1f, 0.1f, 1.0f),
        new D3D11Button(
            600, 200, 100, 100,
            joj::Color(0.0f, 1.0f, 0.0f, 1.0f),
            nullptr
        )
    );
    JOJ_LOG_IF_FAIL(m_canvas->create(device));
    m_canvas->set_hovered_color(joj::Color(0.23f, 0.23f, 0.23f, 1.0f));

    m_widgets.push_back(m_canvas);
}

void joj::D3D11GUI::update(const f32 dt, const i32 xmouse, const i32 ymouse, const b8 clicked)
{
    m_widgets[0]->update(xmouse, ymouse, clicked);
    /*
    if (xmouse < 0 || ymouse < 0)
        return;

    const f32 window_width = 800.0f;
    const f32 window_height = 600.0f;

    f32 ndc_x = 2.0f * static_cast<f32>(xmouse) / window_width - 1.0f;
    f32 ndc_y = 1.0f - 2.0f * static_cast<f32>(ymouse) / window_height;

    i32 hovered_index = -1;
    i32 left_index = -1;
    i32 right_index = -1;
    i32 top_index = -1;
    i32 bottom_index = -1;
    i32 index = 0;
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it, ++index)
    {
        auto& widget = *it;

        if (widget->is_hovered(xmouse, ymouse))
        {
            hovered_index = index;
        }

        if (widget->on_left_edge(xmouse, ymouse) && clicked)
        {
            left_index = index;
        }

        if (widget->on_right_edge(xmouse, ymouse) && clicked)
        {
            right_index = index;
        }

        if (widget->on_top_edge(xmouse, ymouse) && clicked)
        {
            top_index = index;
        }

        if (widget->on_bottom_edge(xmouse, ymouse) && clicked)
        {
            bottom_index = index;
        }
    }

    if (hovered_index != -1 && hovered_index < m_widgets.size())
    {
        m_widgets[hovered_index]->should_update();
    }

    if (left_index != -1 && left_index < m_widgets.size())
    {
        JDEBUG("On left edge of widget %d", left_index);
    }

    if (right_index != -1 && right_index < m_widgets.size())
    {
        JDEBUG("On right edge of widget %d", right_index);
    }

    if (top_index != -1 && top_index < m_widgets.size())
    {
        JDEBUG("On top edge of widget %d", top_index);
    }

    if (bottom_index != -1 && bottom_index < m_widgets.size())
    {
        JDEBUG("On bottom edge of widget %d", bottom_index);
    }
    */
}

void joj::D3D11GUI::draw(CommandList& cmd_list)
{
    m_widgets[0]->draw(cmd_list);
    /*
    for (auto& widget : m_widgets)
    {
        widget->draw(cmd_list);
    }
    */
}

void joj::D3D11GUI::shutdown()
{
}

#endif // JPLATFORM_WINDOWS