#include "app3d_test.h"

#include "joj/core/logger.h"
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine/engine.h"
#include <math.h>
#include "joj/math/jvector3.h"
#include "joj/joj.h"
#include "joj/resources/obj_loader.h"

#include "joj/events/event_manager.h"

// Read file
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // rand()

joj::JFloat3 cross(const joj::JFloat3& v1, const joj::JFloat3& v2) {
    return joj::JFloat3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

joj::JFloat3 normalize(const joj::JFloat3& v) {
    f32 len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.0f)
        return joj::JFloat3(v.x / len, v.y / len, v.z / len);

    return joj::JFloat3(0.0f, 0.0f, 0.0f); // Retorna um vetor nulo se o comprimento for zero
}

joj::JFloat3 calculateFaceNormal(const joj::JFloat3& v0, const joj::JFloat3& v1, const joj::JFloat3& v2)
{
    joj::JFloat3 edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
    joj::JFloat3 edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
    return normalize(cross(edge1, edge2));
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

    const char* filename1 = "models/cube1.txt";
    const char* filename2 = "models/customCube.obj";
    const char* filename3 = "models/cubeNew1.txt";
    MeshData meshData;
    // load_custom_format_with_flat_shading(filename1, meshData);
    // load_custom_format(filename1, meshData);
    // load_obj_format(filename2, meshData);
    // load_obj_format_new(filename2, meshData);
    // load_new_custom_format(filename3, meshData);
    joj::OBJLoader loader;
    joj::InternalMesh* mesh = nullptr;
    mesh = loader.load("models/customCube.obj");
    if (mesh)
    {
        for (size_t i = 0; i < mesh->positions.size(); ++i)
        {
            joj::Vertex::PosColorNormal vertex;
            vertex.pos = mesh->positions[i];

            // Se a cor estiver disponível, usa a cor
            if (i < mesh->colors.size())
            {
                vertex.color = mesh->colors[i];
            }
            else
            {
                // Se não houver cor, você pode definir uma cor padrão
                vertex.color = joj::JFloat4(1.0f, 1.0f, 1.0f, 1.0f); // Cor branca por padrão
            }

            // Normal já está presente
            if (i < mesh->normals.size())
            {
                vertex.normal = mesh->normals[i];
            }
            else
            {
                // Se não houver normal, defina uma normal padrão (por exemplo, (0, 0, 0))
                vertex.normal = joj::JFloat3(0.0f, 0.0f, 0.0f);
            }

            // Adiciona o vértice ao vetor de vértices
            meshData.vertices.push_back(vertex);
        }

        // Agora, copie os índices diretamente de InternalMesh para MeshData
        meshData.indices = mesh->indices;

        m_vertex_buffer.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
            sizeof(joj::Vertex::PosColorNormal) * meshData.vertices.size(), meshData.vertices.data());
        JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

        m_index_buffer.setup(sizeof(u32) * meshData.indices.size(), meshData.indices.data());
        JOJ_LOG_IF_FAIL(m_index_buffer.create(m_renderer->get_device()));

        m_constant_buffer.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
        JOJ_LOG_IF_FAIL(m_constant_buffer.create(m_renderer->get_device()));

        m_light_buffer.setup(joj::calculate_cb_byte_size(sizeof(LightCB)), nullptr);
        JOJ_LOG_IF_FAIL(m_light_buffer.create(m_renderer->get_device()));

        total_indices = meshData.indices.size();
    }
}

void App3DTest::init()
{
    joj::EventManager::instance().subscribe(joj::EventType::KeyPressed, [](const joj::JEvent& event) {
            auto& keyEvent = static_cast<const joj::KeyPressedEvent&>(event);
            JDEBUG("Keypressed! %c", keyEvent.key);
        }
    );

    joj::EventManager::instance().subscribe(joj::EventType::WindowClose, [](const joj::JEvent& event) {
        auto& windowCloseEvent = static_cast<const joj::WindowCloseEvent&>(event);
        JDEBUG("windowCloseEvent!");
        }
    );

    joj::EventManager::instance().subscribe(joj::EventType::MouseClicked, [](const joj::JEvent& event) {
        auto& mouseClicked = static_cast<const joj::MouseClickedEvent&>(event);
        switch (mouseClicked.button)
        {
        case joj::Buttons::BUTTON_LEFT:
            JINFO("BUTTON_LEFT!");
            break;
        case joj::Buttons::BUTTON_MIDDLE:
            JINFO("BUTTON_MIDDLE!");
            break;
        case joj::Buttons::BUTTON_RIGHT:
            JINFO("BUTTON_RIGHT!");
            break;
        }
        }
    );

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

    static f32 rotation = 0.0f;
    // Update the rotation variable each frame.
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
        rotation += 360.0f;

    static f32 angle = 0.0f;
    angle += 0.01f;
    {
        m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
        joj::JMatrix4x4 W = DirectX::XMMatrixRotationY(rotation); // XMMatrixIdentity();
        joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
        joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
        joj::JMatrix4x4 WVP = W * V * P;

        ConstantBuffer cb{};
        XMStoreFloat4x4(&cb.worldMatrix, XMMatrixTranspose(W));

        XMStoreFloat4x4(&cb.wvp, XMMatrixTranspose(WVP));

        // joj::JMatrix4x4 I = DirectX::XMMatrixIdentity();
        // XMStoreFloat4x4(&cb.worldMatrix, XMMatrixTranspose(I));
        XMStoreFloat4x4(&cb.viewMatrix, XMMatrixTranspose(V));
        XMStoreFloat4x4(&cb.projectionMatrix, XMMatrixTranspose(P));
        
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
        lightBuffer.diffuseColor = joj::JVector4(0.0f, 0.7f, 0.7f, 1.0);
        lightBuffer.lightDirection = joj::JVector3(0.0f, 0.0f, 1.0f);
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

void App3DTest::load_custom_format_with_flat_shading(const std::string& filename, MeshData& mesh)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::vector<joj::JVector3> positions;
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
        else if (type == "f")
        {
            u32 a, b, c;
            iss >> a >> b >> c;

            // Pega as posições dos vértices
            joj::JVector3 v0 = positions[a];
            joj::JVector3 v1 = positions[b];
            joj::JVector3 v2 = positions[c];

            // Calcula o vetor normal da face (produto vetorial)
            joj::JVector3 edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
            joj::JVector3 edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

            joj::JVector3 normal = {
                edge1.y * edge2.z - edge1.z * edge2.y,
                edge1.z * edge2.x - edge1.x * edge2.z,
                edge1.x * edge2.y - edge1.y * edge2.x
            };

            // Normaliza a normal
            f32 length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            if (length > 0.0f)
            {
                normal.x /= length;
                normal.y /= length;
                normal.z /= length;
            }

            // Cria os vértices com a mesma normal para cada face
            joj::JVector4 color = { 0.5f, 0.5f, 0.5f, 1.0f }; // Cinza

            mesh.vertices.push_back({ v0, color, normal });
            mesh.vertices.push_back({ v1, color, normal });
            mesh.vertices.push_back({ v2, color, normal });

            // Atualiza os índices (agora temos que usar novos índices para cada face)
            mesh.indices.push_back((u32)mesh.vertices.size() - 3);
            mesh.indices.push_back((u32)mesh.vertices.size() - 2);
            mesh.indices.push_back((u32)mesh.vertices.size() - 1);
        }
    }

    file.close();

    // Print normals
    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        std::cout << "Normal " << i << ": "
            << mesh.vertices[i].normal.x << " "
            << mesh.vertices[i].normal.y << " "
            << mesh.vertices[i].normal.z << std::endl;
    }

    // Write MeshData into a new file
    std::ofstream f("output.txt");
    if (!f.is_open())
    {
        std::cout << "Failed to open output.txt file\n";
        return;
    }

    f << "Vertices: " << mesh.vertices.size() << "\n";
    for (const auto& v : mesh.vertices)
    {
        f << "v " << v.pos.x << " " << v.pos.y << " " << v.pos.z << "\n";
    }

    f << "Normals: " << mesh.vertices.size() << "\n";
    for (const auto& v : mesh.vertices)
    {
        f << "vn " << v.normal.x << " " << v.normal.y << " " << v.normal.z << "\n";
    }

    f << "Faces: " << mesh.indices.size() << "\n";
    u32 index = 0;
    for (const i32 i : mesh.indices)
    {
        f << "index " << index++ << ": " << i << "\n";
    }
}

void App3DTest::load_obj_format_new(const std::string& filename, MeshData& mesh)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::vector<joj::JVector3> temp_positions;
    std::vector<joj::JVector3> temp_normals;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        // Vértices
        if (prefix == "v") {
            joj::JVector3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        // Normais
        else if (prefix == "vn") {
            joj::JVector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Faces
        else if (prefix == "f") {
            std::string vertexData;
            // Lê os três vértices da face
            for (int i = 0; i < 3; ++i) {
                iss >> vertexData;
                std::istringstream vertexStream(vertexData);
                std::string v, t, n;
                unsigned int posIndex = 0, texIndex = 0, normIndex = 0;

                // Lê o índice do vértice
                std::getline(vertexStream, v, '/');
                if (!v.empty()) posIndex = std::stoi(v) - 1;

                // Lê o índice da coordenada de textura (não usado neste exemplo)
                if (std::getline(vertexStream, t, '/')) {
                    if (!t.empty()) texIndex = std::stoi(t) - 1;
                }

                // Lê o índice da normal
                if (std::getline(vertexStream, n)) {
                    if (!n.empty()) normIndex = std::stoi(n) - 1;
                }

                joj::Vertex::PosColorNormal vertex = {};
                if (posIndex < temp_positions.size()) vertex.pos = temp_positions[posIndex];
                if (normIndex < temp_normals.size()) vertex.normal = temp_normals[normIndex];

                // Verifica se o vértice já foi adicionado e usa o índice existente
                auto it = std::find(mesh.vertices.begin(), mesh.vertices.end(), vertex);
                if (it != mesh.vertices.end()) {
                    mesh.indices.push_back(static_cast<unsigned int>(std::distance(mesh.vertices.begin(), it)));
                }
                else {
                    mesh.vertices.push_back(vertex);
                    mesh.indices.push_back(static_cast<unsigned int>(mesh.vertices.size() - 1));
                }
            }
        }
    }

    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        std::cout << "Normal " << i << ": "
            << mesh.vertices[i].normal.x << " "
            << mesh.vertices[i].normal.y << " "
            << mesh.vertices[i].normal.z << std::endl;
        mesh.vertices[i].color = { 0.5f, 0.5f, 0.5f, 1.0f };
    }

    // Write MeshData into a new file
    std::ofstream f("outputOBJ.txt");
    if (!f.is_open())
    {
        std::cout << "Failed to open output.txt file\n";
        return;
    }

    f << "Vertices: " << mesh.vertices.size() << "\n";
    for (const auto& v : mesh.vertices)
    {
        f << "v " << v.pos.x << " " << v.pos.y << " " << v.pos.z << "\n";
    }

    f << "Normals: " << mesh.vertices.size() << "\n";
    for (const auto& v : mesh.vertices)
    {
        f << "vn " << v.normal.x << " " << v.normal.y << " " << v.normal.z << "\n";
    }

    f << "Faces: " << mesh.indices.size() << "\n";
    u32 index = 0;
    for (const i32 i : mesh.indices)
    {
        f << "index " << index++ << ": " << i << "\n";
    }
}

void App3DTest::load_new_custom_format(const std::string& filename, MeshData& mesh)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::vector<joj::JVector3> positions;
    std::vector<joj::JVector3> normals;
    std::vector<std::vector<u32>> faces;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            joj::JVector3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (type == "vn") {
            joj::JVector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f") {
            std::vector<u32> face;
            u32 idx;
            while (iss >> idx) {
                face.push_back(idx); // Ajuste para índice baseado em 0
            }
            faces.push_back(face);
        }
    }

    // Preencher os vértices e índices
    for (const auto& face : faces) {
        for (size_t i = 0; i < face.size(); ++i) {
            unsigned int idx = face[i];
            joj::Vertex::PosColorNormal vertex;
            vertex.pos = positions[idx];
            vertex.color = { 0.5f, 0.5f, 0.5f, 1.0f }; // Cor cinza
            vertex.normal = normals[idx];
            mesh.vertices.push_back(vertex);
            mesh.indices.push_back(mesh.vertices.size() - 1);
        }
    }
}