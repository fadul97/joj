#include "gui_test.h"

#include "joj/engine/engine.h"
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"

struct Vertex
{
    f32 x, y;
};

struct ColorBuffer
{
    f32 r, g, b, a;
};

GUITest::GUITest()
{
}

GUITest::~GUITest()
{

}

void hex_to_f32_RGB(u32 hexColor, f32& r, f32& g, f32& b) {
    r = ((hexColor >> 16) & 0xFF) / 255.0f;
    g = ((hexColor >> 8) & 0xFF) / 255.0f;
    b = (hexColor & 0xFF) / 255.0f;
}

void GUITest::setup_quad_buffers_and_shaders()
{
    m_shader.compile_vertex_shader_from_file(
        "shaders/GUITest.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(m_renderer->get_device()));

    m_shader.compile_pixel_shader_from_file(
        "shaders/GUITest.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(m_renderer->get_device()));

    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());

    joj::InputDesc layout[] =
    {
        { "POSITION", 0, joj::DataFormat::R32G32_FLOAT, 0,  0, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : layout)
    {
        m_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_layout.create(m_renderer->get_device(), m_shader.get_vertex_shader()));
    m_layout.bind(m_renderer->get_cmd_list());

    Vertex vertices[] =
    {
        { -0.5f, -0.5f }, // Bottom-left
        {  0.5f, -0.5f }, // Bottom-right
        { -0.5f,  0.5f }, // Top-left
        {  0.5f,  0.5f }  // Top-right
    };

    u32 indices[] =
    {
        0, 2, 1,  // First triangle (CCW)
        1, 2, 3   // Second triangle (CCW)
    };

    m_vb.setup(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write,
        sizeof(joj::Vertex::PosColor) * 4, vertices);
    JOJ_LOG_IF_FAIL(m_vb.create(m_renderer->get_device()));

    m_ib.setup(sizeof(u32) * 6, indices);
    JOJ_LOG_IF_FAIL(m_ib.create(m_renderer->get_device()));

    m_cb.setup(joj::calculate_cb_byte_size(sizeof(ColorBuffer)), nullptr);
    JOJ_LOG_IF_FAIL(m_cb.create(m_renderer->get_device()));

    m_renderer->set_primitive_topology(joj::PrimitiveTopology::TRIANGLE_LIST);
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

void GUITest::init()
{
    JDEBUG("Size of Win32Window: %d", sizeof(joj::Win32Window));     // 136
    JDEBUG("Size of Win32Input: %d", sizeof(joj::Win32Input));       // 8
    JDEBUG("Size of D3D11Renderer: %d", sizeof(joj::D3D11Renderer)); // 152
    setup_quad_buffers_and_shaders();
}

// Check whether current mouse position is within a rectangle
i32 GUITest::regionhit(i32 x, i32 y, i32 w, i32 h)
{
    if (uistate.mousex < x ||
        uistate.mousey < y ||
        uistate.mousex >= x + w ||
        uistate.mousey >= y + h)
        return 0;
    return 1;
}

i32 GUITest::button(i32 id, i32 x, i32 y, const f32 r, const f32 g, const f32 b, const f32 a)
{
    // Check whether the button should be hot
    if (regionhit(x, y, 64, 48))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
            uistate.activeitem = id;
    }

    // If no widget has keyboard focus, take it
    if (uistate.kbditem == 0)
        uistate.kbditem = id;

    // If we have keyboard focus, show it
    if (uistate.kbditem == id)
        draw_rect(x - 6, y - 6, 84, 68, 1.0f, 0.0f, 0.0f, 1.0f);

    // Render button 
    draw_rect(x + 8, y + 8, 64, 48, 0.0f, 0.0f, 0.0f, 1.0f);
    if (uistate.hotitem == id)
    {
        if (uistate.activeitem == id)
        {
            // Button is both 'hot' and 'active'
            draw_rect(x + 2, y + 2, 64, 48, r, g, b, 1.0f);
        }
        else
        {
            // Button is merely 'hot'
            draw_rect(x, y, 64, 48, r, g, b, a);
        }
    }
    else
    {
        // button is not hot, but it may be active    
        draw_rect(x, y, 64, 48, r, g, b, a);
    }

    // If we have keyboard focus, we'll need to process the keys
    if (uistate.kbditem == id)
    {
        switch (uistate.keyentered)
        {
        case joj::KEY_TAB:
            // If tab is pressed, lose keyboard focus.
            // Next widget will grab the focus.
            uistate.kbditem = 0;
            // If shift was also pressed, we want to move focus
            // to the previous widget instead.
            if (uistate.keymod)
                uistate.kbditem = uistate.lastwidget;
            // Also clear the key so that next widget
            // won't process it
            uistate.keyentered = 0;
            break;
        case joj::KEY_ENTER:
            // Had keyboard focus, received return,
            // so we'll act as if we were clicked.
            return 1;
        }
    }

    uistate.lastwidget = id;

    // If button is hot and active, but mouse button is not
    // down, the user must have clicked the button.
    if (uistate.mousedown == 0 &&
        uistate.hotitem == id &&
        uistate.activeitem == id)
        return 1;

    // Otherwise, no clicky.
    return 0;
}

// Simple scroll bar IMGUI widget
i32 GUITest::slider(i32 id, i32 x, i32 y, i32 max, i32& value)
{
    // Calculate mouse cursor's relative y offset
    i32 ypos = ((256 - 16) * value) / max;

    f32 r = 0.0f;
    f32 g = 0.0f;
    f32 b = 0.0f;

    // Check for hotness
    if (regionhit(x + 8, y + 8, 16, 255))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
            uistate.activeitem = id;
    }

    // If no widget has keyboard focus, take it
    if (uistate.kbditem == 0)
        uistate.kbditem = id;

    // If we have keyboard focus, show it
    if (uistate.kbditem == id)
        draw_rect(x - 4, y - 4, 40, 280, 1.0f, 0.0f, 0.0f, 1.0f);

    // Render the scrollbar
    draw_rect(x, y, 32, 256 + 16, 0.46f, 0.46f, 0.46f, 1.0f);

    if (uistate.activeitem == id || uistate.hotitem == id)
    {
        draw_rect(x + 8, y + 8 + ypos, 16, 16, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        hex_to_f32_RGB(0xaaaaaa, r, g, b);
        draw_rect(x + 8, y + 8 + ypos, 16, 16, r, g, b, 1.0f);
    }

    // If we have keyboard focus, we'll need to process the keys
    if (uistate.kbditem == id)
    {
        switch (uistate.keyentered)
        {
        case joj::KEY_TAB:
            // If tab is pressed, lose keyboard focus.
            // Next widget will grab the focus.
            uistate.kbditem = 0;
            // If shift was also pressed, we want to move focus
            // to the previous widget instead.
            if (uistate.keymod)
                uistate.kbditem = uistate.lastwidget;
            // Also clear the key so that next widget
            // won't process it
            uistate.keyentered = 0;
            break;
        case joj::KEY_UP:
            // Slide slider up (if not at zero)
            if (value > 0)
            {
                value--;
                return 1;
            }
            break;
        case joj::KEY_DOWN:
            // Slide slider down (if not at max)
            if (value < max)
            {
                value++;
                return 1;
            }
            break;
        }
    }

    uistate.lastwidget = id;

    // Update widget value
    if (uistate.activeitem == id)
    {
        i32 mousepos = uistate.mousey - (y + 8);
        if (mousepos < 0) mousepos = 0;
        if (mousepos > 255) mousepos = 255;
        i32 v = (mousepos * max) / 255;
        if (v != value)
        {
            value = v;
            return 1;
        }
    }

    return 0;
}

// Prepare for IMGUI code
void GUITest::imgui_prepare()
{
    uistate.hotitem = 0;
}

void GUITest::imgui_finish()
{
    if (uistate.mousedown == 0)
    {
        uistate.activeitem = 0;
    }
    else
    {
        if (uistate.activeitem == 0)
            uistate.activeitem = -1;
    }
    // If no widget grabbed tab, clear focus
    if (uistate.keyentered == joj::KEY_TAB)
        uistate.kbditem = 0;

    // Clear the entered key
    uistate.keyentered = 0;
}

void GUITest::render(const f32 dt)
{
    static i32 bgcolor = 0x77;

    static f32 r = 0.5f;
    static f32 g = 0.5f;
    static f32 b = 0.5f;
    // draw_rect(0, 0, 640, 480, r, g, b, 1.0f);

    imgui_prepare();

    button(GEN_ID, 50, 50, r, g, b, 1.0f);

    button(GEN_ID, 150, 50, r, g, b, 1.0f);

    if (button(GEN_ID, 50, 150, r, g, b, 1.0f))
    {
        r = 1.0f;
        g = 1.0f;
        b = 0.0f;
    }

    if (button(GEN_ID, 150, 150, r, g, b, 1.0f))
        joj::Engine::close();

    i32 slidervalue = bgcolor & 0xff;
    if (slider(GEN_ID, 500, 40, 255, slidervalue))
    {
        bgcolor = (bgcolor & 0xffff00) | slidervalue;
    }

    slidervalue = ((bgcolor >> 10) & 0x3f);
    if (slider(GEN_ID, 550, 40, 63, slidervalue))
    {
        bgcolor = (bgcolor & 0xff00ff) | (slidervalue << 10);
    }

    slidervalue = ((bgcolor >> 20) & 0xf);
    if (slider(GEN_ID, 600, 40, 15, slidervalue))
    {
        bgcolor = (bgcolor & 0x00ffff) | (slidervalue << 20);
    }

    imgui_finish();
}

void GUITest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    {
        if (m_input->is_button_down(joj::BUTTON_LEFT))
            uistate.mousedown = 1;
        if (m_input->is_button_up(joj::BUTTON_LEFT))
            uistate.mousedown = 0;

        uistate.mousex = m_input->get_xmouse();
        uistate.mousey = m_input->get_ymouse();

        if (m_input->is_key_pressed(joj::KEY_TAB))
            uistate.keyentered = joj::KEY_TAB;

        if (m_input->is_key_pressed(joj::KEY_ENTER))
            uistate.keyentered = joj::KEY_ENTER;

        if (m_input->is_key_up(joj::KEY_SHIFT))
            uistate.keymod = 0;

        if (m_input->is_key_pressed(joj::KEY_SHIFT))
            uistate.keymod = joj::KEY_SHIFT;

        if (m_input->is_key_pressed(joj::KEY_UP))
            uistate.keyentered = joj::KEY_UP;

        if (m_input->is_key_pressed(joj::KEY_DOWN))
            uistate.keyentered = joj::KEY_DOWN;
    }

    m_dt = dt;
}

void GUITest::draw_quad()
{
    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_layout.bind(m_renderer->get_cmd_list());

    u32 stride = sizeof(Vertex);
    u32 offset = 0;

    m_vb.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_ib.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

    ColorBuffer rectColor = { 1.0f, 1.0f, 0.0f, 1.0f };
    m_cb.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.update(m_renderer->get_cmd_list(), rectColor);

    m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);
}

void GUITest::draw_rect(const i32 x, const i32 y, const i32 w, const i32 h, const f32 r, const f32 g, const f32 b, const f32 a)
{
    constexpr i32 screen_width = 800;
    constexpr i32 screen_height = 600;

    // Normalize coordinates
    f32 left = (2.0f * x) / static_cast<f32>(screen_width) - 1.0f;
    f32 right = (2.0f * (x + w)) / static_cast<f32>(screen_width) - 1.0f;
    f32 top = 1.0f - (2.0f * y) / static_cast<f32>(screen_height);
    f32 bottom = 1.0f - (2.0f * (y + h)) / static_cast<f32>(screen_height);

    Vertex vertices[] =
    {
        { left,  bottom }, // 2: Bottom-left
        { right, bottom }, // 3: Bottom-right
        { left,  top  },   // 0: Top-left
        { right, top  }    // 1: Top-right
    };

    m_vb.update(m_renderer->get_cmd_list(), vertices);

    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_layout.bind(m_renderer->get_cmd_list());

    u32 stride = sizeof(Vertex);
    u32 offset = 0;
    m_vb.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_ib.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

    ColorBuffer rectColor = { r, g, b, a };
    m_cb.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.update(m_renderer->get_cmd_list(), rectColor);

    m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);
}

void GUITest::draw()
{
    m_renderer->clear();

    m_renderer->disable_depth_test();
    render(m_dt);
    // draw_rect(uistate.mousex - 32, uistate.mousey - 24, 150, 100, 1.0f, 0.0f, 0.0f, 1.0f);
    m_renderer->enable_depth_test();

    m_renderer->swap_buffers();
}

void GUITest::shutdown()
{
}