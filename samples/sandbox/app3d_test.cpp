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

// ------------------------------------------------------------------------------------------------

inline std::vector<u8> load_binary_data(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open binary buffer file.");

    file.seekg(0, std::ios::end); // Go to the end of the file
    size_t size = file.tellg(); // Get the file size
    std::cout << "File size: " << size << std::endl;
    file.seekg(0, std::ios::beg); // Go back to the beginning of the file

    std::vector<u8> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);

    return data;
}

inline std::vector<joj::Vector3> load_positions_from_buffer(const std::vector<u8>& buffer, size_t byteOffset, size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(buffer.data() + byteOffset);
    return std::vector<joj::Vector3>(data, data + count);
}

inline std::vector<joj::Vector3> load_normals_from_buffer(const std::vector<u8>& buffer, size_t byteOffset, size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(buffer.data() + byteOffset);
    return std::vector<joj::Vector3>(data, data + count);
}

inline std::vector<uint16_t> load_indices_from_buffer(const std::vector<u8>& buffer, size_t byteOffset, size_t count)
{
    const uint16_t* data = reinterpret_cast<const uint16_t*>(buffer.data() + byteOffset);
    return std::vector<uint16_t>(data, data + count);
}

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
    m_camera.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_camera.look_at(m_camera.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_camera.get_up());
}

void App3DTest::build_buffers()
{
    // Load binary data from file
    const char* filename = "models/Lantern.gltf";
    if (m_model_importer.load(filename) != joj::ErrorCode::OK)
        return;

    m_scene = m_model_importer.get_scene();
    if (m_scene == nullptr)
        return;

    // Print Scene submeshes count
    JOJ_DEBUG("Submeshes count: %d", m_scene->get_submesh_count());

    // Print scene info
    // m_scene->print_info();
    // m_scene->write_vertices_and_indices_to_file("Scene_Vertices_Indices.txt");

    // Print vertices, indices and submesh count
    // std::cout << "Vertices count: " << m_scene->get_vertex_count() << std::endl; // 4145
    // std::cout << "Indices count: " << m_scene->get_index_count() << std::endl;   // 16182
    // std::cout << "Submesh count: " << m_scene->get_submesh_count() << std::endl; // 3

    std::vector<joj::Vertex::ColorTanPosNormalTex> vertices_data;
    /*
    vertices_data.reserve(m_scene->get_vertex_count());
    const auto& vertices = m_scene->get_vertex_data();
    for (const auto& vertex : vertices)
    {
        joj::Vertex::ColorTanPosNormalTex v;
        v.pos = vertex.pos;
        v.color = vertex.color;
        v.normal = vertex.normal;
        vertices_data.push_back(v);
    }
    */
   std::cout << "Inserting vertices data..." << std::endl;
   vertices_data.insert(vertices_data.begin(), m_scene->get_vertex_data().begin(), m_scene->get_vertex_data().end());

    // Print vertices size
    const i32 vertex_count = m_scene->get_vertex_count();
    std::cout << "Vertices size: " << vertices_data.size() << std::endl;

    // Write vertices to file
    /*
    std::ofstream vertices_file("APPvertices.txt");
    if (vertices_file.is_open())
    {
        i32 count = 0;
        vertices_file << "Vertices count: " << vertices_data.size() << std::endl;
        vertices_file << "    => Vertices:\n";
        for (const auto& vertex : vertices_data)
        {
            // Fix precision to 4 decimal places
            vertices_file << "        " << count++ << ": " << std::fixed << std::setprecision(4) << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
        }
        vertices_file.close();
    }
    */

    std::vector<u16> indices_data;
    /*
    indices_data.reserve(m_scene->get_index_count());
    const auto& indices = m_scene->get_index_data();
    for (const auto& index : indices)
    {
        indices_data.push_back(index);
    }
    */
    std::cout << "Inserting indices data..." << std::endl;
    indices_data.insert(indices_data.begin(), m_scene->get_index_data().begin(), m_scene->get_index_data().end());

    // Write indices to file
    /*
    std::ofstream indices_file("APPindices.txt");
    if (indices_file.is_open())
    {
        indices_file << "Indices count: " << indices_data.size() << std::endl;
        indices_file << "    => Indices:\n";
        for (const auto& index : indices_data)
        {
            indices_file << "        " << index << "\n";
        }
        indices_file.close();
    }
    */

    // Print indices size
    const i32 index_count = m_scene->get_index_count();
    std::cout << "Indices size: " << indices_data.size() << std::endl;

    m_vb = joj::D3D11VertexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    m_ib = joj::D3D11IndexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());

    if (m_vb.create(joj::BufferUsage::Default,
        joj::CPUAccessType::None,
        vertex_count * sizeof(joj::Vertex::ColorTanPosNormalTex),
        vertices_data.data()) != joj::ErrorCode::OK)
        return;

    if (m_ib.create(joj::BufferUsage::Default,
    joj::CPUAccessType::None,
    index_count * sizeof(u16),
    indices_data.data()) != joj::ErrorCode::OK)
    return;

    // Create shader
    m_shader = joj::D3D11Shader(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_shader.compile_vertex_shader(joj::ShaderLibrary::ColorTanPosNormalTexWithCamera, "VS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;

    if (m_shader.compile_pixel_shader(joj::ShaderLibrary::ColorTanPosNormalTexWithCamera, "PS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;

    std::vector<joj::InputDesc> layout =
    {
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0,  0, joj::InputClassification::PerVertexData, 0 },
        { "TANGENT",  0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 16, joj::InputClassification::PerVertexData, 0 },
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0, 32, joj::InputClassification::PerVertexData, 0 },
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 44, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 56, joj::InputClassification::PerVertexData, 0 },
    };

    if (m_shader.create_input_layout(layout) != joj::ErrorCode::OK)
        return;

    m_cb = joj::D3D11ConstantBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_cb.create(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write, joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr) != joj::ErrorCode::OK)
        return;
    
    m_lightcb = joj::D3D11ConstantBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_lightcb.create(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write, joj::calculate_cb_byte_size(sizeof(LightBuffer)), nullptr) != joj::ErrorCode::OK)
        return;
}

void App3DTest::init()
{
    setup_camera();
    build_buffers();
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

static f32 rotation = 0.0f;
void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    if (m_input->is_key_pressed('T'))
        toogle_movement_speed_decrease = !toogle_movement_speed_decrease;

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
            m_ib.bind(joj::DataFormat::R16_UINT, offset);
            m_shader.bind();
            // m_scene->draw_mesh_index(m_renderer, m_submesh_index);
            m_scene->draw(m_renderer);
        }

        m_cb.bind_to_vertex_shader(0, 1);
        {
            joj::JMatrix4x4 worldMatrix = DirectX::XMMatrixIdentity();
            worldMatrix = DirectX::XMMatrixRotationY(rotation * 2.0f) * DirectX::XMMatrixTranslation(-20.0f, 0.0f, 0.0f);
            
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
            m_ib.bind(joj::DataFormat::R16_UINT, offset);
            m_shader.bind();
            m_scene->draw_mesh_index(m_renderer, m_submesh_index);
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
    f32 speed = dt * 10.0f;

    if (toogle_movement_speed_decrease)
        speed = dt * 1.0f;

    if (m_input->is_key_down(joj::KEY_CONTROL))
        speed += dt * 40.0f;

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