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
    // Load GLTF file
    m_beautiful_game_importer;
    if (m_beautiful_game_importer.load("models/Lantern.gltf") != joj::ErrorCode::OK)
        return;

    m_model;
    m_beautiful_game_importer.get_meshes(m_model.meshes); // tamanho de m_model.meshes é 3
    // m_model.print_info();

    m_mesh;
    m_beautiful_game_importer.setup_mesh(m_model.meshes[0], m_mesh);
    m_beautiful_game_importer.setup_aggregated_meshes(m_mesh);

    std::vector<joj::Vertex::ColorTanPosNormalTex> vertices_data;
    vertices_data.reserve(m_mesh.get_vertex_count());
    const auto& vertices = m_mesh.get_vertex_data();
    for (const auto& vertex : vertices)
    {
        joj::Vertex::ColorTanPosNormalTex v;
        v.pos = vertex.pos;
        v.color = vertex.color;
        v.normal = vertex.normal;
        vertices_data.push_back(v);
    }
    
    std::vector<u16> indices_data;
    indices_data.reserve(m_mesh.get_index_count());
    const auto& indices = m_mesh.get_index_data();
    for (const auto& index : indices)
    {
        indices_data.push_back(index);
    }

    /*
    for (const auto& vertex : vertices)
    {
        std::cout << "Vertex: " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << std::endl;
        std::cout << "Normal: " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << std::endl;
    }
    */

    // Create vertex buffer
    m_use_new_vertex = false;
    if (m_use_new_vertex)
    {
        std::cout << "Using new vertex format." << std::endl;
        const u32 vertices_size = static_cast<u32>(m_mesh.get_vertex_count());
        m_vertex_cout = vertices_size;
        JOJ_DEBUG("Vertices count: %d", m_vertex_cout);
        m_vb = joj::D3D11VertexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
        if (m_vb.create(joj::BufferUsage::Default, joj::CPUAccessType::None,
            sizeof(joj::Vertex::ColorTanPosNormalTex) * vertices_size, &m_mesh.get_vertex_data()) != joj::ErrorCode::OK)
            return;
    
        // Create index buffer
        const u32 indices_size = static_cast<u32>(m_mesh.get_index_count());
        m_index_count = indices_size;
        JOJ_DEBUG("Indices count: %d", m_index_count);
        m_ib = joj::D3D11IndexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
        if (m_ib.create(joj::BufferUsage::Default, joj::CPUAccessType::None, sizeof(u16) * indices_size, &m_mesh.get_index_data()) != joj::ErrorCode::OK)
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
            { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0, 28, joj::InputClassification::PerVertexData, 0 },
            { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 40, joj::InputClassification::PerVertexData, 0 },
            { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 52, joj::InputClassification::PerVertexData, 0 },
        };

        if (m_shader.create_input_layout(layout) != joj::ErrorCode::OK)
            return;
    }
    else
    {
        std::cout << "Using old vertex format." << std::endl;
        const u32 vertices_size = static_cast<u32>(vertices_data.size());
        m_vertex_cout = vertices_size;
        JOJ_DEBUG("Vertices count: %d", m_vertex_cout);
        m_vb = joj::D3D11VertexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
        if (m_vb.create(joj::BufferUsage::Default, joj::CPUAccessType::None,
            sizeof(joj::Vertex::ColorTanPosNormalTex) * vertices_size, vertices_data.data()) != joj::ErrorCode::OK)
            return;
    
        // Create index buffer
        const u32 indices_size = static_cast<u32>(indices.size());
        m_index_count = indices_size;
        JOJ_DEBUG("Indices count: %d", m_index_count);
        m_ib = joj::D3D11IndexBuffer(m_renderer->get_device(), m_renderer->get_cmd_list());
        if (m_ib.create(joj::BufferUsage::Default, joj::CPUAccessType::None, sizeof(u16) * indices_size, indices.data()) != joj::ErrorCode::OK)
            return;

        // Create shader
        /*
        m_shader = joj::D3D11Shader(m_renderer->get_device(), m_renderer->get_cmd_list());
        if (m_shader.compile_vertex_shader(joj::ShaderLibrary::PosColorNormalWithLightRotation, "VS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;
        
        if (m_shader.compile_pixel_shader(joj::ShaderLibrary::PosColorNormalWithLightRotation, "PS", joj::ShaderModel::Default) != joj::ErrorCode::OK)
        return;
        
       std::vector<joj::InputDesc> layout =
       {
           { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
           { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
           { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 28, joj::InputClassification::PerVertexData, 0 },
        };
        */
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
    }

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
    create_buffers_for_model(m_model, m_renderer);

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

        draw_model(m_model, joj::float4x4_identity());

        draw_modelNew(m_model, joj::float4x4_identity());
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
}

void App3DTest::process_mouse_input(const f32 dt)
{
    f32 speed = dt * 10.0f;

    if (m_input->is_key_down(joj::KEY_CONTROL))
        speed = dt * 40.0f;

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

void App3DTest::draw_model(const joj::GLTFModel& model, const joj::JFloat4x4& parent_transform)
{
    // Iterar sobre todas as meshes do modelo
    for (size_t mesh_index = 0; mesh_index < model.meshes.size(); ++mesh_index)
    {
        const auto& mesh = model.meshes[mesh_index];

        // Obter os buffers correspondentes à mesh
        if (mesh_index >= model.vbs.size() || mesh_index >= model.ibs.size())
        {
            JOJ_ERROR(joj::ErrorCode::FAILED, "Mesh index out of bounds in vbs/ibs.");
            continue;
        }

        // Encontrar o nó correspondente a essa mesh
        joj::JMatrix4x4 worldMatrix = DirectX::XMLoadFloat4x4(&parent_transform);

        for (const auto& node : model.nodes)
        {
            if (node.mesh_index == mesh_index)
            {
                // Aplicar transformações do nó (posição, rotação e escala)
                joj::JMatrix4x4 translation = DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
                DirectX::XMFLOAT4 rotationQuaternion = {node.rotation.x, node.rotation.y, node.rotation.z, node.rotation.w};
                joj::JMatrix4x4 rotation = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotationQuaternion));
                joj::JMatrix4x4 scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);

                worldMatrix = DirectX::XMMatrixMultiply(scale, rotation);
                worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translation);
                worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMLoadFloat4x4(&parent_transform));
                break;
            }
        }

        m_cb.bind_to_vertex_shader(0, 1);
        // Calcular matrizes de câmera e projeção
        joj::JMatrix4x4 W = worldMatrix;
        joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
        joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
        joj::JMatrix4x4 WVP = W * V * P;

        // Atualizar constantes do shader
        ConstantBuffer cbData = {};
        DirectX::XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
        DirectX::XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
        DirectX::XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
        DirectX::XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
        m_cb.update(cbData);

        // Bind dos buffers antes do draw
        u32 stride;
        if (m_use_new_vertex)
            stride = sizeof(joj::Vertex::ColorTanPosNormalTex);
        else
            stride = sizeof(joj::Vertex::ColorTanPosNormalTex);
        constexpr u32 offset = 0;

        m_vb.bind(0, 1, &stride, &offset);
        m_ib.bind(joj::DataFormat::R16_UINT, offset);

        // Bind do shader
        m_shader.bind();

        // Renderizar cada primitiva dentro da mesh
        for (const auto& primitive : mesh.primitives)
        {
            u32 index_count = get_index_count_for_primitive(primitive);
            m_renderer->draw_indexed(index_count, 0, 0);
            /*
            if (!mesh.name.empty())
            std::cout << "[DRAW CALL]: " << mesh.name << " (" << index_count << " indices)" << std::endl;
            else
            std::cout << "[DRAW CALL]: (Unnamed Mesh) (" << index_count << " indices)" << std::endl;
            */
        }
    }
}

void App3DTest::draw_modelNew(const joj::GLTFModel& model, const joj::JFloat4x4& parent_transform)
{
    joj::JMatrix4x4 worldMatrix = DirectX::XMLoadFloat4x4(&parent_transform);

    m_cb.bind_to_vertex_shader(0, 1);
    // Calcular matrizes de câmera e projeção
    joj::JMatrix4x4 W = worldMatrix;
    joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
    joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
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

    // Bind do shader
    m_shader.bind();

    // Renderizar cada primitiva dentro da mesh
    for (const auto& submesh : m_mesh.get_submeshes())
    {
        m_renderer->draw_indexed(submesh.index_count, submesh.index_start, submesh.vertex_start);
    }
}

u32 App3DTest::get_index_count_for_primitive(const joj::GLTFPrimitive& primitive)
{
    i32 index_accessor = primitive.indices_acessor;
    if (index_accessor == -1)
        return 0;

    const auto& index_accessor_data = m_beautiful_game_importer.get_accessor(index_accessor);
    return index_accessor_data.count;
}

void App3DTest::create_buffers_for_model(joj::GLTFModel& model, joj::IRenderer* renderer)
{
    model.vbs.clear();
    model.ibs.clear();

    i32 mesh_index = 0;
    for (const auto& mesh : model.meshes)
    {
        std::vector<joj::GLTFVertex> vertices;
        std::vector<u16> indices;

        for (const auto& primitive : mesh.primitives)
        {
            i32 index_position_accessor = primitive.position_acessor;
            i32 index_indices_accessor = primitive.indices_acessor;
            i32 index_normal_accessor = primitive.normal_acessor;
            i32 index_color_accessor = primitive.color_acessor;

            if (index_position_accessor == -1 || index_indices_accessor == -1 || index_normal_accessor == -1)
                continue;

            // Ler dados usando os novos métodos
            std::vector<joj::Vector3> positions = m_beautiful_game_importer.read_buffer<joj::Vector3>(index_position_accessor);
            std::vector<u16> indices_data = m_beautiful_game_importer.read_buffer<u16>(index_indices_accessor);
            std::vector<joj::Vector3> normals = m_beautiful_game_importer.read_buffer<joj::Vector3>(index_normal_accessor);
            std::vector<joj::Vector4> colors;
            if (index_color_accessor != -1)
            {
                colors = m_beautiful_game_importer.read_buffer<joj::Vector4>(index_color_accessor);
            }

            for (size_t i = 0; i < positions.size(); i++)
            {
                joj::GLTFVertex vertex;
                vertex.pos = positions[i];
                vertex.color = (i < colors.size()) ? colors[i] : joj::Vector4(1, 1, 1, 1);
                vertex.normal = (i < normals.size()) ? normals[i] : joj::Vector3(0, 0, 1); // Normal padrão caso não exista
                vertices.push_back(vertex);
            }

            indices.insert(indices.end(), indices_data.begin(), indices_data.end());
        }

        std::string node_name = "(Unnamed Node)";
        for (const auto& node : model.nodes)
        {
            if (node.mesh_index == mesh_index)
            {
                node_name = node.name; // Supondo que sua estrutura SceneNode tenha um método get_name()
                break; // Encontrou, pode sair do loop
            }
        }

        std::cout << "Building mesh: " << node_name << "(node_name) OR " << mesh.name << "(mesh.name)" << std::endl;
        // Print vertices count
        std::cout << "Vertices count: " << vertices.size() << std::endl;
        // Print indices count
        std::cout << "Indices count: " << indices.size() << std::endl;

        auto vb = std::make_unique<joj::D3D11VertexBuffer>(renderer->get_device(), renderer->get_cmd_list());
        if (vb->create(joj::BufferUsage::Default, joj::CPUAccessType::None,
            sizeof(joj::GLTFVertex) * vertices.size(), vertices.data()) != joj::ErrorCode::OK)
        {
            JOJ_ERROR(joj::ErrorCode::FAILED, "Falha ao criar vertex buffer!");
            continue;
        }

        auto ib = std::make_unique<joj::D3D11IndexBuffer>(renderer->get_device(), renderer->get_cmd_list());
        if (ib->create(joj::BufferUsage::Default, joj::CPUAccessType::None,
            sizeof(u16) * indices.size(), indices.data()) != joj::ErrorCode::OK)
        {
            JOJ_ERROR(joj::ErrorCode::FAILED, "Falha ao criar index buffer!");
            continue;
        }

        // Adicionar ao modelo
        model.vbs.push_back(std::move(vb));
        model.ibs.push_back(std::move(ib));
        ++mesh_index;
    }

    // Print model vbs and ibs count
    std::cout << "Total vertex buffers: " << model.vbs.size() << std::endl;
    std::cout << "Total index buffers: " << model.ibs.size() << std::endl;
}