#include "gui_test.h"

#include "joj/engine/engine.h"
#include "joj/core/jmacros.h"

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

void GUITest::init()
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
        { -1.0f, -1.0f }, // Bottom-left
        {  1.0f, -1.0f }, // Bottom-right
        { -1.0f,  1.0f }, // Top-left
        {  1.0f,  1.0f }  // Top-right
    };

    u32 indices[] = { 0, 1, 2, 1, 3, 2 };

    m_vb.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(Vertex) * 4, vertices);
    JOJ_LOG_IF_FAIL(m_vb.create(m_renderer->get_device()));

    m_ib.setup(sizeof(u32) * 6, indices);
    JOJ_LOG_IF_FAIL(m_ib.create(m_renderer->get_device()));

    m_cb.setup(joj::calculate_cb_byte_size(sizeof(ColorBuffer)), nullptr);
    JOJ_LOG_IF_FAIL(m_cb.create(m_renderer->get_device()));

    m_renderer->set_primitive_topology(joj::PrimitiveTopology::TRIANGLE_LIST);
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

void GUITest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();
}

void GUITest::draw()
{
    m_renderer->clear();

    {
        m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
        m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
        m_layout.bind(m_renderer->get_cmd_list());

        u32 stride = sizeof(Vertex);
        u32 offset = 0;
        
        m_vb.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
        m_ib.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);
        
        ColorBuffer rectColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        m_cb.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
        m_cb.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
        m_cb.update(m_renderer->get_cmd_list(), rectColor);

        m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);
    }

    m_renderer->swap_buffers();
}

void GUITest::shutdown()
{
}