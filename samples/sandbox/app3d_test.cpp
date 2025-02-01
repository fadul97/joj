#include "app3d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"
#include <math.h>

App3DTest::App3DTest()
{
}

App3DTest::~App3DTest()
{
}

void App3DTest::build_shader()
{
    JINFO("Building shader...");

    m_shader.compile_vertex_shader_from_file(
        "shaders/SimpleModel.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(m_renderer->get_device()));

    m_shader.compile_pixel_shader_from_file(
        "shaders/SimpleModel.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(m_renderer->get_device()));
}

void App3DTest::build_input_layout()
{
    JINFO("Building input layout...");

    joj::InputDesc sprite_layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : sprite_layout)
    {
        m_input_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_input_layout.create(m_renderer->get_device(), m_shader.get_vertex_shader()));
}

void App3DTest::build_buffers()
{
    JINFO("Building buffers...");

    joj::Vertex::PosColor quad_vertices[] =
    {
        { joj::JFloat3{ -0.5f, -0.5f, 0.0f }, joj::JFloat4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // Bottom Left
        { joj::JFloat3{  0.5f, -0.5f, 0.0f }, joj::JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Right
        { joj::JFloat3{ -0.5f,  0.5f, 0.0f }, joj::JFloat4{ 0.0f, 0.0f, 1.0f, 1.0f } }, // Top Left
        { joj::JFloat3{  0.5f,  0.5f, 0.0f }, joj::JFloat4{ 1.0f, 1.0f, 0.0f, 1.0f } }, // Top Right
    };

    m_vertex_buffer.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(joj::Vertex::PosColor) * 4, quad_vertices);
    JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

    u32 indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    m_index_buffer.setup(sizeof(u32) * 6, indices);

    JOJ_LOG_IF_FAIL(m_index_buffer.create(m_renderer->get_device()));
}

void App3DTest::init()
{
    build_shader();
    build_input_layout();
    build_buffers();
}

void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();
}

void App3DTest::draw()
{
    m_renderer->clear(0.4f, 0.4f, 0.4f, 1.0f);

    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_input_layout.bind(m_renderer->get_cmd_list());

    u32 stride = sizeof(joj::Vertex::PosColor);
    u32 offset = 0;

    m_vertex_buffer.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_index_buffer.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

    m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);

    m_renderer->swap_buffers();
}

void App3DTest::shutdown()
{
}