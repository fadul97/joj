#include "app3d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"
#include <math.h>

// Read file
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // rand()

App3DTest::App3DTest()
{
    m_last_mouse_pos = { 0, 0 };
}

App3DTest::~App3DTest()
{
}

void App3DTest::setup_camera()
{
    m_camera.update_view_matrix();
    m_camera.set_pos(0.0f, 5.0f, -15.0f);
    m_camera.update_view_matrix();
    m_camera.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_camera.update_view_matrix();
    m_camera.look_at(m_camera.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_camera.get_up());
    m_camera.update_view_matrix();
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

struct MeshData
{
    std::vector<joj::Vertex::PosColor> vertices;
    std::vector<u32> indices;
};

static u32 total_indices = 0;
void App3DTest::build_buffers()
{
    JINFO("Building buffers...");

    const char* filename = "models/monkey1.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::vector<joj::JFloat3> positions;
    std::string line;
    MeshData mesh;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            f32 x, y, z;
            iss >> x >> y >> z;
            positions.push_back({ x, y, z });
        }
        else if (type == "f")
        {
            u32 a, b, c;
            iss >> a >> b >> c;
            mesh.indices.push_back(a); // Convertendo de 1-based para 0-based
            mesh.indices.push_back(b);
            mesh.indices.push_back(c);
        }
    }

    // Criando os vértices com cores aleatórias
    for (const auto& pos : positions)
    {
        joj::JFloat4 color =
        {
            // 1.0f, 1.0f, 0.0f,
            static_cast<f32>(rand()) / RAND_MAX, // Vermelho aleatório
            static_cast<f32>(rand()) / RAND_MAX, // Verde aleatório
            static_cast<f32>(rand()) / RAND_MAX, // Azul aleatório
            1.0f  // Alfa (sempre 1)
        };

        mesh.vertices.push_back({ pos, color });
    }

    m_vertex_buffer.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(joj::Vertex::PosColor) * mesh.vertices.size(), mesh.vertices.data());
    JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

    m_index_buffer.setup(sizeof(u32) * mesh.indices.size(), mesh.indices.data());
    JOJ_LOG_IF_FAIL(m_index_buffer.create(m_renderer->get_device()));

    m_constant_buffer.setup(joj::calculate_cb_byte_size(sizeof(CameraCB)), nullptr);
    JOJ_LOG_IF_FAIL(m_constant_buffer.create(m_renderer->get_device()));

    total_indices = mesh.indices.size();
}

void App3DTest::init()
{
    setup_camera();
    build_shader();
    build_input_layout();
    build_buffers();
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    process_mouse_input(dt);
}

void App3DTest::draw()
{
    m_renderer->clear(0.4f, 0.4f, 0.4f, 1.0f);

    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_input_layout.bind(m_renderer->get_cmd_list());

    {
        m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
        CameraCB cb{};
        cb.view = m_camera.get_view();
        cb.proj = m_camera.get_proj();
        auto view_proj = XMLoadFloat4x4(&cb.view) * XMLoadFloat4x4(&cb.proj);
        DirectX::XMStoreFloat4x4(&cb.view_proj, XMMatrixTranspose(view_proj));
        auto world = joj::matrix4x4_identity();
        auto wvp = world * XMLoadFloat4x4(&cb.view) * XMLoadFloat4x4(&cb.proj);
        DirectX::XMStoreFloat4x4(&cb.wvp, XMMatrixTranspose(wvp));
        cb.eye_pos_w = m_camera.get_pos();
        m_constant_buffer.update(m_renderer->get_cmd_list(), cb);
    }

    u32 stride = sizeof(joj::Vertex::PosColor);
    u32 offset = 0;

    m_vertex_buffer.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_index_buffer.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

    m_renderer->get_cmd_list().device_context->DrawIndexed(total_indices, 0, 0);

    m_renderer->swap_buffers();
}

void App3DTest::shutdown()
{
}

void App3DTest::on_mouse_down(joj::Buttons button, i32 x, i32 y)
{
    m_last_mouse_pos.x = x;
    m_last_mouse_pos.y = y;

    SetCapture(m_window->get_data().handle);
}

void App3DTest::on_mouse_up(joj::Buttons button, i32 x, i32 y)
{
    ReleaseCapture();
}

void App3DTest::on_mouse_move(WPARAM button_state, i32 x, i32 y)
{
    if ((button_state & MK_RBUTTON) != 0)
    {
        f32 dx = DirectX::XMConvertToRadians(0.25f * static_cast<f32>(x - m_last_mouse_pos.x));
        f32 dy = DirectX::XMConvertToRadians(0.25f * static_cast<f32>(y - m_last_mouse_pos.y));

        m_camera.pitch(dy);
        m_camera.rotateY(dx);
    }

    m_last_mouse_pos.x = x;
    m_last_mouse_pos.y = y;
}

void App3DTest::process_mouse_input(const f32 dt)
{
    const f32 speed = dt * 20.0f;

    if (m_input->is_key_down('W'))
        m_camera.walk(speed);

    if (m_input->is_key_down(joj::KEY_S))
        m_camera.walk(-speed);

    if (m_input->is_key_down(joj::KEY_A))
        m_camera.strafe(-speed);

    if (m_input->is_key_down(joj::KEY_D))
        m_camera.strafe(speed);

    m_camera.update_view_matrix();
}