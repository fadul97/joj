#include "app3d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"
#include <math.h>
#include "logger.h"

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
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 28, joj::InputClassification::PerVertexData, 0 }
    };

    for (auto& l : sprite_layout)
    {
        m_input_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_input_layout.create(m_renderer->get_device(), m_shader.get_vertex_shader()));
}

static u32 total_indices = 0;
void App3DTest::build_buffers()
{
    JINFO("Building buffers...");

    const char* filename = "models/cube1.txt";
    MeshData mesh;
    load_custom_format(filename, mesh);
    // load_obj_format(filename, mesh);

    m_vertex_buffer.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(joj::Vertex::PosColorNormal) * mesh.vertices.size(), mesh.vertices.data());
    JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

    m_index_buffer.setup(sizeof(u32) * mesh.indices.size(), mesh.indices.data());
    JOJ_LOG_IF_FAIL(m_index_buffer.create(m_renderer->get_device()));

    m_constant_buffer.setup(joj::calculate_cb_byte_size(sizeof(CameraCB)), nullptr);
    JOJ_LOG_IF_FAIL(m_constant_buffer.create(m_renderer->get_device()));

    m_light_buffer.setup(joj::calculate_cb_byte_size(sizeof(LightCB)), nullptr);
    JOJ_LOG_IF_FAIL(m_light_buffer.create(m_renderer->get_device()));

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

static f32 angleY = 0.0f;
void App3DTest::draw()
{
    m_renderer->clear(0.4f, 0.4f, 0.4f, 1.0f);

    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_input_layout.bind(m_renderer->get_cmd_list());

    angleY += 0.001f;
    {
        m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
        CameraCB cb{};

        // World Matrix
        joj::JMatrix4x4 rotationY = DirectX::XMMatrixRotationY(angleY);
        joj::JMatrix4x4 world = joj::matrix4x4_identity();
        world = DirectX::XMMatrixMultiply(world, rotationY);
        DirectX::XMStoreFloat4x4(&cb.world, XMMatrixTranspose(world));

        // Inverse World Matrix
        joj::JMatrix4x4 inverseWorld = DirectX::XMMatrixInverse(nullptr, world);
        DirectX::XMStoreFloat4x4(&cb.inverse_world, inverseWorld);

        // View Matrix
        joj::JMatrix4x4 view = DirectX::XMLoadFloat4x4(&m_camera.get_view());
        DirectX::XMStoreFloat4x4(&cb.view, XMMatrixTranspose(view));
        
        // Proj Matrix
        joj::JMatrix4x4 proj = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
        DirectX::XMStoreFloat4x4(&cb.proj, XMMatrixTranspose(proj));
        
        // ViewProj Matrix
        joj::JMatrix4x4 view_proj = XMLoadFloat4x4(&cb.view) * XMLoadFloat4x4(&cb.proj);
        DirectX::XMStoreFloat4x4(&cb.view_proj, XMMatrixTranspose(view_proj));
        
        // WVP Matrix
        auto wvp = world * view * proj;
        DirectX::XMStoreFloat4x4(&cb.wvp, XMMatrixTranspose(wvp));
        
        // Camera position
        cb.eye_pos_w = m_camera.get_pos();
        
        // Update CameraCB
        m_constant_buffer.update(m_renderer->get_cmd_list(), cb);

        /*
        JFloat4 ambient;
		JFloat4 diffuse;
		JFloat4 specular;
		JFloat3 direction;
		f32 pad; // Padding
        */
        m_light_buffer.bind_to_pixel_shader(m_renderer->get_cmd_list(), 1, 1);
        LightCB lightBuffer{};
        lightBuffer.dir_light.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
        lightBuffer.dir_light.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
        lightBuffer.dir_light.specular = { 0.7f, 0.7f, 0.7f, 1.0f };
        lightBuffer.dir_light.direction = { 1.0f, -1.0f, 0.0f };
        m_light_buffer.update(m_renderer->get_cmd_list(), lightBuffer);
    }

    u32 stride = sizeof(joj::Vertex::PosColorNormal);
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

void App3DTest::load_custom_format(const std::string& filename, MeshData& mesh)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::vector<joj::JFloat3> positions;
    std::vector<joj::JFloat3> normals;
    std::string line;

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
        else if (type == "vn")
        {
            f32 x, y, z;
            iss >> x >> y >> z;
            normals.push_back({ x, y, z });
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
    for (size_t i = 0; i < positions.size() && i < normals.size(); ++i)
    {
        joj::JFloat4 color =
        {
            0.5f, 0.5f, 0.5f,
            // static_cast<f32>(rand()) / RAND_MAX, // Vermelho aleatório
            // static_cast<f32>(rand()) / RAND_MAX, // Verde aleatório
            // static_cast<f32>(rand()) / RAND_MAX, // Azul aleatório
            1.0f  // Alfa (sempre 1)
        };

        joj::JFloat3 normal = (i < normals.size()) ? normals[i] : joj::JFloat3{ 0.0f, 0.0f, 0.0f };  // Garantindo que temos uma normal

        mesh.vertices.push_back({ positions[i], color, normal });
    }

    // Print normals
    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        std::cout << "Normal " << i << ": " << mesh.vertices[i].normal.x << " " << mesh.vertices[i].normal.y << " " << mesh.vertices[i].normal.z << std::endl;
    }
}

void App3DTest::load_obj_format(const std::string& filename, MeshData& mesh)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        JERROR(joj::ErrorCode::ERR_FILE_OPENED_FAILED,
            "Failed to open file '%s'.", filename.c_str());
    }

    std::vector<joj::JFloat3> temp_positions;
    std::vector<joj::JFloat3> temp_normals;
    std::vector<joj::JFloat2> temp_texcoords;

    std::unordered_map<std::string, u32> index_map;
    std::string line;

    while (std::getline(fin, line))
    {
        std::istringstream iss(line);

        // Identificar o prefixo da linha
        std::string prefix;
        iss >> prefix;

        // Vertices
        if (prefix == "v")
        {
            joj::JFloat3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        // Normais
        else if (prefix == "vn")
        {
            joj::JFloat3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Coordenadas de textura (caso precise)
        else if (prefix == "vt")
        {
            joj::JFloat2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        // Faces
        else if (prefix == "f")
        {
            std::string vertexData;
            // Lê cada um dos 3 vértices da face
            for (int i = 0; i < 3; ++i)
            {
                iss >> vertexData;
                std::istringstream vertexStream(vertexData);
                std::string v, t, n;
                u32 posIndex = 0, texIndex = 0, normIndex = 0;

                // Lê o índice do vértice
                std::getline(vertexStream, v, '/');
                if (!v.empty()) posIndex = std::stoi(v) - 1;

                // Lê o índice da coordenada de textura (opcional)
                if (std::getline(vertexStream, t, '/'))
                {
                    if (!t.empty()) texIndex = std::stoi(t) - 1;
                }

                // Lê o índice da normal
                if (std::getline(vertexStream, n))
                {
                    if (!n.empty()) normIndex = std::stoi(n) - 1;
                }

                joj::Vertex::PosColorNormal vertex = {};
                if (posIndex < temp_positions.size()) vertex.pos = temp_positions[posIndex];
                if (normIndex < temp_normals.size()) vertex.normal = temp_normals[normIndex];

                // Aqui estamos usando a normal, mas se quiser usar texturas, pode também atribuir texcoord
                if (texIndex < temp_texcoords.size())
                {
                    // Atribua as coordenadas de textura, se necessário
                }

                // Verifica se o vértice já foi adicionado, se sim, utiliza o índice
                auto it = std::find(mesh.vertices.begin(), mesh.vertices.end(), vertex);
                if (it != mesh.vertices.end())
                {
                    mesh.indices.push_back(static_cast<u32>(std::distance(mesh.vertices.begin(), it)));
                }
                else
                {
                    mesh.vertices.push_back(vertex);
                    mesh.indices.push_back(static_cast<u32>(mesh.vertices.size() - 1));
                }
            }
        }
    }

    // Adicionando cores aleatórias aos vértices
    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        joj::JFloat4 color =
        {
            0.5f, 0.5f, 0.5f,
            // static_cast<f32>(rand()) / RAND_MAX,  // Vermelho aleatório
            // static_cast<f32>(rand()) / RAND_MAX,  // Verde aleatório
            // static_cast<f32>(rand()) / RAND_MAX,  // Azul aleatório
            1.0f  // Alfa (sempre 1)
        };

        mesh.vertices[i].color = color;
    }

    // Exibindo as normais para depuração
    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        std::cout << "Normal " << i << ": " << mesh.vertices[i].normal.x << " " << mesh.vertices[i].normal.y << " " << mesh.vertices[i].normal.z << std::endl;
    }
}
