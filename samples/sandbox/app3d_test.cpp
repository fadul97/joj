#include "app3d_test.h"

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
#include <cstdlib> // rand()
#include <unordered_map>
#include "joj/renderer/shader_library.h"
#include "joj/utils/json_parser.h"

// ------------------------------------------------------------------------------------------------
#include "joj/resources/gltf/gltf_importer.h"
#include "joj/core/logger.h"

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
    m_camera.update_view_matrix();
    m_camera.set_pos(0.0f, 5.0f, -15.0f);
    m_camera.update_view_matrix();
    m_camera.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_camera.update_view_matrix();
    m_camera.look_at(m_camera.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_camera.get_up());
    m_camera.update_view_matrix();
}

void App3DTest::build_buffers()
{
    m_gltf_importer = joj::OLDGLTFImporter("models/BiggerCubeSRT.gltf");
    if (m_gltf_importer.load() == joj::ErrorCode::OK)
    {
        // m_gltf_importer.print_scene_info();
        // m_gltf_importer.print_node_info();
        // m_gltf_importer.print_mesh_info();
        m_gltf_importer.print_vertex_data();
        // m_gltf_importer.print_animation_data();
        
        //JOJ_DEBUG("================= Translation data =================");
        //m_gltf_importer.print_translation_data();
        
        //JOJ_DEBUG("================= Rotation data =================");
        //m_gltf_importer.print_rotation_data();
        
        //JOJ_DEBUG("================= Scale data =================");
        //m_gltf_importer.print_scale_data();
        
        m_gltf_importer.setup_animations();
        //JOJ_DEBUG("================= Animation data =================");
        // m_gltf_importer.print_animation_data();
    }

    const size_t vertices_byteOffset = m_gltf_importer.m_positions_byte_offset;
    const size_t vertices_count = m_gltf_importer.m_positions_count;

    const size_t normals_byteOffset = m_gltf_importer.m_normals_byte_offset;
    const size_t normals_count = m_gltf_importer.m_normals_count;

    const size_t indices_byteOffset = m_gltf_importer.m_indices_byte_offset;
    const size_t indices_count = m_gltf_importer.m_indices_count;

    /*
    // Print normals
    i32 count = 0;
    for (const auto& normal : normals)
    JOJ_INFO("Normal %d: %.3f, %.3f, %.3f", count++, normal.x, normal.y, normal.z);
    
    // Print positions
    for (const auto& pos : positions)
    JOJ_INFO("Position: %.3f, %.3f, %.3f", pos.x, pos.y, pos.z);
    
    // Print indices
    for (const auto& index : indices)
    JOJ_INFO("Index: %d", index);
    */

    // Array of Vector4 colors
    joj::Vector4 colors[4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f }
    };

    // Loop positions and normals to create vertices
    std::vector<joj::Vertex::PosColorNormal> vertices;
    for (size_t i = 0; i < m_gltf_importer.m_positions.size(); ++i)
    {
        joj::Vertex::PosColorNormal vertex;
        vertex.pos = m_gltf_importer.m_positions[i];
        vertex.color = colors[1];
        vertex.normal = m_gltf_importer.m_normals[i];
        vertices.push_back(vertex);
    }
    // Size of PosColorNormal: 40

    // Create vertex buffer
    const u32 vertices_size = static_cast<u32>(vertices.size());
    m_vertex_cout = vertices_size;
    //JOJ_DEBUG("Vertices count: %d", m_vertex_cout);
    m_vb = joj::D3D11VertexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_vb.create(joj::BufferUsage::Default, joj::CPUAccessType::None,
        sizeof(joj::Vertex::PosColorNormal) * vertices_size, vertices.data()) != joj::ErrorCode::OK)
        return;

    // Create index buffer
    const u32 indices_size = static_cast<u32>(m_gltf_importer.m_indices.size());
    m_index_count = indices_size;
    //JOJ_DEBUG("Indices count: %d", m_index_count);
    m_ib = joj::D3D11IndexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_ib.create(joj::BufferUsage::Default, joj::CPUAccessType::None, sizeof(u16) * indices_size, m_gltf_importer.m_indices.data()) != joj::ErrorCode::OK)
        return;

    // Create shader
    m_shader = joj::D3D11Shader(m_renderer->get_device(), m_renderer->get_cmd_list());
    if (m_shader.compile_vertex_shader(joj::ShaderLibrary::PosColorNormalWithLightRotation, "VS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;

    if (m_shader.compile_pixel_shader(joj::ShaderLibrary::PosColorNormalWithLightRotation, "PS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;

    std::vector<joj::InputDesc> layout =
    {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 28, joj::InputClassification::PerVertexData, 0 }
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

    joj::GLTFImporter box_importer;
    if JOJ_FAILED(box_importer.load("models/Box.gltf"))
        JOJ_ERROR(joj::ErrorCode::FAILED, "Failed to load GLTF file.");

    joj::GLTFImporter riggedSimple_importer;
    if JOJ_FAILED(riggedSimple_importer.load("models/RiggetSimple.gltf"))
        JOJ_ERROR(joj::ErrorCode::FAILED, "Failed to load GLTF file.");

    joj::GLTFImporter simpleSkin_importer;
    if JOJ_FAILED(simpleSkin_importer.load("models/SimpleSkin.gltf"))
        JOJ_ERROR(joj::ErrorCode::FAILED, "Failed to load GLTF file.");

    joj::OLDGLTFNode cubeNode;

    // Definindo a posição inicial do cubo
    cubeNode.position = joj::Vector3(0.0f, 0.0f, 0.0f);  // Posição inicial no centro da cena

    // Definindo a rotação inicial (sem rotação, identidade)
    cubeNode.rotation = joj::Vector4(0.0f, 0.0f, 0.0f, 1.0f);  // Quaternions: identidade

    // Definindo a escala inicial
    cubeNode.scale = joj::Vector3(1.0f, 1.0f, 1.0f);  // Escala padrão de 1

    // Armazenar o nó na lista de nós, ou apenas utilizá-lo diretamente
    m_nodes.push_back(cubeNode);

    m_animations = m_gltf_importer.get_animations();

    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

static f32 rotation = 0.0f;
void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    const f32 speed = 10.0f * dt;;
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
        rotation += 360.0f;

    process_mouse_input(dt);

    static b8 loop_animation = false;
    if (m_input->is_key_pressed('T'))
        loop_animation = !loop_animation;

    static f32 animation_time = 0.0f;
    animation_time += dt;
    if (!m_animations.empty())
    {
        joj::OLDGLTFAnimation& animation = m_animations[0];

        // Supondo que o nó do cubo esteja na posição 0
        joj::OLDGLTFNode& node = m_nodes[0];

        f32 animation_speed = 60.0f;

        // Aplique a animação ao nó com loop
        joj::apply_all_animations(animation, animation_time * animation_speed, node, loop_animation);

        if (!loop_animation && animation_time > animation.channels[0].keyframes.back().time)  // Tempo da última keyframe
        {
            animation_time = animation.channels[0].keyframes.back().time;  // Garantir que a animação pare na última posição
        }
    }
}

void App3DTest::draw()
{
    m_renderer->begin_frame();
    {
        m_cb.bind_to_vertex_shader(0, 1);
        {
            joj::JMatrix4x4 W = DirectX::XMMatrixRotationY(rotation); // XMMatrixIdentity();
            joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
            joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
            joj::JMatrix4x4 WVP = W * V * P;
    
            ConstantBuffer cbData = {};
            XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
            XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
            XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
            XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
            m_cb.update(cbData);
        }

        m_lightcb.bind_to_pixel_shader(1, 1);
        {
            LightBuffer lightBuffer;
            lightBuffer.diffuseColor = joj::JFloat4(1.0f, 0.7f, 0.7f, 1.0);
            lightBuffer.lightDirection = joj::JFloat3(0.0f, 0.0f, 1.0f);
            m_lightcb.update(lightBuffer);
        }

        // Bind and Draw the rect
        constexpr u32 stride = sizeof(joj::Vertex::PosColorNormal);
        constexpr u32 offset = 0;
        m_vb.bind(0, 1, &stride, &offset);
        m_ib.bind(joj::DataFormat::R16_UINT, offset);
        m_shader.bind();
        m_renderer->draw_indexed(m_index_count, 0, 0);

        {
            m_cb.bind_to_vertex_shader(0, 1);
            {
                // Aplique as animações ao nó (no caso, o primeiro nó da cena)
                joj::OLDGLTFNode& node = m_nodes[0];  // Supondo que seja o cubo

                DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationRollPitchYaw(node.rotation.x, node.rotation.y, node.rotation.z);
                // Crie uma matriz de transformação para o nó
                // joj::JMatrix4x4 W = DirectX::XMMatrixTranslation(node.position.x, node.position.y, node.position.z);
                joj::JMatrix4x4 W = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z) *
                                    DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionNormalize(rotation)) *
                                    DirectX::XMMatrixTranslation(node.position.x, node.position.y, node.position.z);

                /*
                // Print World matrix for debugging
                for (i32 i = 0; i < 4; ++i)
                {
                    std::cout << "W[" << i << "]: ";
                    for (i32 j = 0; j < 4; ++j)
                    {
                        std::cout << W.r[i].m128_f32[j] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << "\n" << std::endl;
                */
                
                // Matriz de visualização e projeção
                joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
                joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
                joj::JMatrix4x4 WVP = W * V * P;
        
                ConstantBuffer cbData = {};
                XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
                XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
                XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
                XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
                m_cb.update(cbData);
            }

            m_lightcb.bind_to_pixel_shader(1, 1);
            {
                LightBuffer lightBuffer;
                lightBuffer.diffuseColor = joj::JFloat4(1.0f, 0.7f, 0.7f, 1.0);
                lightBuffer.lightDirection = joj::JFloat3(0.0f, 0.0f, 1.0f);
                m_lightcb.update(lightBuffer);
            }

            // Bind and Draw the rect
            constexpr u32 stride = sizeof(joj::Vertex::PosColorNormal);
            constexpr u32 offset = 0;
            m_vb.bind(0, 1, &stride, &offset);
            m_ib.bind(joj::DataFormat::R16_UINT, offset);
            m_shader.bind();
            m_renderer->draw_indexed(m_index_count, 0, 0);
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

void App3DTest::update_animations(const f32 dt)
{
    m_current_time += dt;

    for (auto& animation : m_animations)
    {
        joj::apply_animation(animation, m_current_time, m_nodes[0]); // Only for the cube
    }
}

void App3DTest::process_mouse_input(const f32 dt)
{
    f32 speed = dt * 10.0f;

    if (m_input->is_key_down(joj::KEY_CONTROL))
        speed *= 5;

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