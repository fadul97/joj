#include "app3d_test.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/logger.h"
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine/engine.h"
#include <math.h>
#include "joj/math/jvector3.h"
#include "joj/joj.h"

// Read file
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib> // rand()
#include <unordered_map>
#include "joj/renderer/shader_library.h"
#include "joj/utils/json_parser.h"
#include "joj/core/logger.h"

constexpr f32 MOUSE_MOVEMENT_SPEED = 5.0f;

// ------------------------------------------------------------------------------------------------

App3DTest::App3DTest()
{
    m_last_mouse_pos = { 0, 0 };
    joj_print();
}

App3DTest::~App3DTest()
{
}

void App3DTest::setup_camera()
{
    m_camera.set_pos(0.0f, 5.0f, -15.0f);
    m_camera.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 10000.0f);
    m_camera.look_at(m_camera.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_camera.get_up());
}

void App3DTest::build_buffers()
{
    // Load binary data from file
    const char* filename = "models/AnimSimpleCube.gltf";
    if (m_model_importer.load(filename) != joj::ErrorCode::OK)
        return;

    m_scene = m_model_importer.get_scene();
    if (m_scene == nullptr)
        return;

    // Write submesh info to file
    m_scene->write_submesh_data_to_file("Scene_Submeshes.txt");

    std::vector<joj::Vertex::ColorTanPosNormalTex> vertices_data;
    vertices_data.insert(vertices_data.begin(), m_scene->get_vertex_data().begin(), m_scene->get_vertex_data().end());
    // Print vertices size
    const i32 vertex_count = m_scene->get_vertex_count();
    std::cout << "Vertices size: " << vertices_data.size() << std::endl;

    std::vector<u32> indices_data;
    indices_data.insert(indices_data.begin(), m_scene->get_index_data().begin(), m_scene->get_index_data().end());
    // Print indices size
    const i32 index_count = m_scene->get_index_count();
    std::cout << "Indices size: " << indices_data.size() << std::endl;

    m_vb = joj::D3D11VertexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    m_ib = joj::D3D11IndexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());

    if JOJ_FAILED(m_vb.create(joj::BufferUsage::Default,
        joj::CPUAccessType::None,
        vertex_count * sizeof(joj::Vertex::ColorTanPosNormalTex),
        vertices_data.data()))
    {
        return;
    }

    if JOJ_FAILED(m_ib.create(joj::BufferUsage::Default,
        joj::CPUAccessType::None,
        index_count * sizeof(u32),
        indices_data.data()))
    {
        return;
    }

    // Create shader
    m_shader = joj::D3D11Shader(m_renderer->get_device(), m_renderer->get_cmd_list());
    if JOJ_FAILED(m_shader.compile_vertex_shader(joj::ShaderLibrary::ColorTanPosNormalTexWithCamera, "VS", joj::ShaderModel::Default))
        return;

    if JOJ_FAILED(m_shader.compile_pixel_shader(joj::ShaderLibrary::ColorTanPosNormalTexWithCamera, "PS", joj::ShaderModel::Default))
        return;

    std::vector<joj::InputDesc> layout =
    {
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0,  0, joj::InputClassification::PerVertexData, 0 },
        { "TANGENT",  0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 16, joj::InputClassification::PerVertexData, 0 },
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0, 32, joj::InputClassification::PerVertexData, 0 },
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 44, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 56, joj::InputClassification::PerVertexData, 0 },
    };

    if JOJ_FAILED(m_shader.create_input_layout(layout))
        return;

    m_cb = joj::D3D11ConstantBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if JOJ_FAILED(m_cb.create(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write, joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr))
        return;
    
    m_lightcb = joj::D3D11ConstantBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if JOJ_FAILED(m_lightcb.create(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write, joj::calculate_cb_byte_size(sizeof(LightBuffer)), nullptr))
        return;
}

void App3DTest::init()
{
    setup_camera();
    build_buffers();
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);

    // Print Scene submeshes count
    JOJ_DEBUG("Submeshes count: %d", m_scene->get_submesh_count());
}

static f32 rotation = 0.0f;
void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    if (m_input->is_key_pressed('T'))
        toogle_movement_speed_decrease = !toogle_movement_speed_decrease;

    if (m_input->is_key_pressed('C'))
        m_wireframe = !m_wireframe;

    if (m_input->is_key_pressed('I'))
    {
        m_submesh_index = (m_submesh_index + 1) % m_scene->get_submesh_count();
        std::cout << "Submesh index: " << m_submesh_index << std::endl;
    }

    const f32 speed = 10.0f * dt;;
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
        rotation += 360.0f;
    

    process_mouse_input(dt);
}

void App3DTest::draw()
{
    m_renderer->begin_frame();
    {
        m_renderer->set_rasterizer_state(m_wireframe ? joj::RasterizerState::Wireframe : joj::RasterizerState::Solid);

        // Per frame updates
        m_lightcb.bind_to_pixel_shader(1, 1);
        {
            LightBuffer lightBuffer;
            lightBuffer.diffuseColor = joj::JFloat4(0.7f, 0.7f, 0.7f, 1.0);
            lightBuffer.lightDirection = joj::JFloat3(0.0f, 0.0f, 1.0f);
            m_lightcb.update(lightBuffer);
        }

        // Per object updates
        m_cb.bind_to_vertex_shader(0, 1);
        {
            joj::JMatrix4x4 worldMatrix = DirectX::XMMatrixIdentity();
            // W = DirectX::XMMatrixRotationY(rotation * 2.0f) * DirectX::XMMatrixTranslation(2.0f, 2.0f, 0.0f);
            
            // Calcular matrizes de câmera e projeção
            joj::JMatrix4x4 W = worldMatrix;
            joj::JMatrix4x4 V = m_camera.get_view().to_XMMATRIX();
            joj::JMatrix4x4 P = m_camera.get_proj().to_XMMATRIX();
            joj::JMatrix4x4 WVP = W * V * P;

            // Atualizar constantes do shader
            ConstantBuffer cbData = {};
            DirectX::XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
            DirectX::XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
            DirectX::XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
            DirectX::XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
            m_cb.update(cbData);

            constexpr u32 stride = sizeof(joj::Vertex::ColorTanPosNormalTex);
            constexpr u32 offset = 0;
            m_vb.bind(0, 1, &stride, &offset);
            m_ib.bind(joj::DataFormat::R32_UINT, offset);
            m_shader.bind();
            // m_scene->draw_mesh_index(m_renderer, m_submesh_index);
            m_scene->draw(m_renderer);
        }
    }
    m_renderer->end_frame();
}

void App3DTest::shutdown()
{
}

void App3DTest::on_mouse_down(joj::Buttons button, i32 x, i32 y)
{
    m_last_mouse_pos.x = x;
    m_last_mouse_pos.y = y;

    SetCapture(m_window->get_data()->handle);
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
    f32 speed = dt * MOUSE_MOVEMENT_SPEED;

    if (toogle_movement_speed_decrease)
        speed *= 0.5f;

    if (m_input->is_key_down(joj::KEY_CONTROL))
        speed *= 4.0f;

    if (m_input->is_key_down('W'))
        m_camera.walk(speed);

    if (m_input->is_key_down(joj::KEY_S))
        m_camera.walk(-speed);

    if (m_input->is_key_down(joj::KEY_A))
        m_camera.strafe(-speed);

    if (m_input->is_key_down(joj::KEY_D))
        m_camera.strafe(speed);
}

#endif // JOJ_PLATFORM_WINDOWS