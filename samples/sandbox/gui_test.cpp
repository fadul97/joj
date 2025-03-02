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

void GUITest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();
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

void GUITest::draw_rect(const i32 x, const i32 y, const i32 w, const i32 h, const i32 r, const i32 g, const i32 b, const i32 a)
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

    ColorBuffer rectColor = { 1.0f, 1.0f, 0.0f, 1.0f };
    m_cb.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
    m_cb.update(m_renderer->get_cmd_list(), rectColor);

    m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);
}

void GUITest::draw()
{
    m_renderer->clear();

    draw_rect(100, 100, 150, 100, 1.0f, 0.0f, 0.0f, 1.0f);

    m_renderer->swap_buffers();
}

void GUITest::shutdown()
{
}