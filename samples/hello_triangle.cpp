#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "joj/math/jmath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <renderer/d3d11/shader_library_d3d11.h>

u32 vertexCount = 0;
u32 indexCount = 0;

using namespace DirectX;
struct Vertex
{
    joj::JFloat3 position; // XMFloat3
    joj::JFloat2 texture;  // XMFloat2
    joj::JFloat3 normal;   // XMFloat3
};

bool operator==(const Vertex& lhs, const Vertex& rhs) {
    return lhs.position.x == rhs.position.x &&
        lhs.position.y == rhs.position.y &&
        lhs.position.z == rhs.position.z &&
        lhs.texture.x == rhs.texture.x &&
        lhs.texture.y == rhs.texture.y &&
        lhs.normal.x == rhs.normal.x &&
        lhs.normal.y == rhs.normal.y &&
        lhs.normal.z == rhs.normal.z;
}

ID3D11InputLayout* gInputLayout = nullptr;
ID3D11SamplerState* m_sampleState = nullptr;
ID3D11Buffer* m_indexBuffer = nullptr;

// Estrutura para o Constant Buffer
struct ConstantBuffer
{
    joj::JFloat4x4 wvp;
    joj::JFloat4x4 worldMatrix;
    joj::JFloat4x4 viewMatrix;
    joj::JFloat4x4 projectionMatrix;
};

struct CameraBufferType
{
    joj::JFloat3 cameraPosition;
    f32 padding;
};

struct LightBuffer
{
    joj::JFloat4 ambientColor;
    joj::JFloat4 diffuseColor;
    joj::JFloat3 lightDirection;
    f32 specularPower;
    joj::JFloat4 specularColor;
};

b8 load_OBJ(const std::string& filename, Vertex*& vertices, u32*& indices,
    u32& vertex_count, u32& index_count)
{
    std::vector<joj::JFloat3> positions;
    std::vector<joj::JFloat2> tex_coords;
    std::vector<joj::JFloat3> normals;
    std::vector<Vertex> vertex_list;
    std::vector<u32> index_list;

    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            joj::JFloat3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "vt")
        {
            joj::JFloat2 tex;
            iss >> tex.x >> tex.y;
            tex_coords.push_back(tex);
        }
        else if (prefix == "vn")
        {
            joj::JFloat3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        }
        else if (prefix == "f")
        {
            std::string vertexData;
            while (iss >> vertexData)
            {
                std::istringstream vertexStream(vertexData);
                std::string v, t, n;
                u32 posIndex = 0, texIndex = 0, normIndex = 0;

                std::getline(vertexStream, v, '/');
                if (!v.empty()) posIndex = std::stoi(v) - 1;

                if (std::getline(vertexStream, t, '/')) {
                    if (!t.empty()) texIndex = std::stoi(t) - 1;
                }

                if (std::getline(vertexStream, n)) {
                    if (!n.empty()) normIndex = std::stoi(n) - 1;
                }

                Vertex vertex = {};
                if (posIndex < positions.size()) vertex.position = positions[posIndex];
                if (texIndex < tex_coords.size()) vertex.texture = tex_coords[texIndex];
                if (normIndex < normals.size()) vertex.normal = normals[normIndex];

                auto it = std::find(vertex_list.begin(), vertex_list.end(), vertex);
                if (it != vertex_list.end()) {
                    index_list.push_back(static_cast<u32>(std::distance(vertex_list.begin(), it)));
                }
                else {
                    vertex_list.push_back(vertex);
                    index_list.push_back(static_cast<u32>(vertex_list.size() - 1));
                }
            }
        }
    }

    file.close();

    // Copiar dados para os arrays de saída
    vertex_count = vertex_list.size();
    index_count = index_list.size();

    vertices = new Vertex[vertex_count];
    std::copy(vertex_list.begin(), vertex_list.end(), vertices);

    indices = new u32[index_count];
    std::copy(index_list.begin(), index_list.end(), indices);

    return true;
}

HelloTriangle::HelloTriangle()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    input = joj::Win32Input();
    timer = joj::Win32Timer();
    renderer = joj::D3D11Renderer();
}

HelloTriangle::~HelloTriangle()
{

}

void HelloTriangle::init()
{
    if (window.create() != joj::ErrorCode::OK)
        return;

    u32 width = 0;
    u32 height = 0;

    window.get_window_size(width, height);
    JDEBUG("Window size: %dx%d", width, height);

    window.get_client_size(width, height);
    JDEBUG("Client size: %dx%d", width, height);

    input.set_window(window.get_data());

    timer.begin_period();

    renderer_print();

    if (renderer.initialize(window.get_data()) != joj::ErrorCode::OK)
        return;

    m_cam.update_view_matrix();
    m_cam.set_pos(0.0f, 5.0f, -20.0f);
    m_cam.update_view_matrix();
    m_cam.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_cam.update_view_matrix();
    m_cam.look_at(m_cam.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_cam.get_up());
    m_cam.update_view_matrix();

    timer.start();

    // Create the vertex array.
    Vertex* vertices = nullptr;

    // Create the index array.
    u32* indices = nullptr;

    if (!load_OBJ("../../../../samples/models/MySpaceShip.obj",
        vertices, indices, vertexCount, indexCount))
        JERROR(joj::ErrorCode::FAILED, "Failed to load model.");

    m_vb.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(Vertex) * vertexCount, vertices);
    m_vb.create(renderer.get_device());

    m_ib.setup(sizeof(u32) * indexCount, indices);
    m_ib.create(renderer.get_device());

    m_mat_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
    m_mat_cb.create(renderer.get_device());

    m_light_cb.setup(joj::calculate_cb_byte_size(sizeof(LightBuffer)), nullptr);
    m_light_cb.create(renderer.get_device());

    m_camera_cb.setup(joj::calculate_cb_byte_size(sizeof(CameraBufferType)), nullptr);
    m_camera_cb.create(renderer.get_device());

    m_shader.compile_vertex_shader(joj::D3D11ShaderLibrary::VertexShaderCode,
        "main", joj::ShaderModel::Default);
    m_shader.create_vertex_shader(renderer.get_device());

    m_shader.compile_pixel_shader(joj::D3D11ShaderLibrary::PixelShaderCode,
        "main", joj::ShaderModel::Default);
    m_shader.create_pixel_shader(renderer.get_device());

    m_tex.create(renderer.get_device(),
        renderer.get_cmd_list(),
        L"../../../../samples/textures/stone.png",
        joj::ImageType::PNG);

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    if (renderer.get_device().device->CreateSamplerState(&samplerDesc, &m_sampleState) != S_OK)
    {
        JERROR(joj::ErrorCode::FAILED, "Failed to create D3D11 Sampler State.");
    }

    // Layout de entrada
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    renderer.get_device().device->CreateInputLayout(layout, 3,
        m_shader.get_vertex_shader().vsblob->GetBufferPointer(),
        m_shader.get_vertex_shader().vsblob->GetBufferSize(), &gInputLayout);

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;
}

void HelloTriangle::update(const f32 dt)
{
    if (input.is_key_pressed('A'))
        JDEBUG("A pressed.");

    if (input.is_key_down(joj::KEY_SPACE))
        JDEBUG("Space down.");

    if (input.is_key_pressed(joj::KEY_ESCAPE))
        loop = false;


    static float rotation = 0.0f;
    // Update the rotation variable each frame.
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
    {
        rotation += 360.0f;
    }

    static float angle = 0.0f;
    angle += 0.01f;

    {
        joj::JMatrix4x4 W = XMMatrixRotationY(rotation); // joj::matrix4x4_identity();
        joj::JMatrix4x4 V = XMLoadFloat4x4(&m_cam.get_view());
        joj::JMatrix4x4 P = XMLoadFloat4x4(&m_cam.get_proj());
        joj::JMatrix4x4 WVP = W * V * P;

        ConstantBuffer cbData = {};
        XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
        XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
        XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
        XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
        m_mat_cb.update(renderer.get_cmd_list(), cbData);
    }

    {
        LightBuffer lightBuffer;
        lightBuffer.ambientColor = joj::JFloat4(0.15f, 0.15f, 0.15f, 1.0f);
        lightBuffer.diffuseColor = joj::JFloat4(1.0f, 1.0f, 1.0f, 1.0);
        lightBuffer.lightDirection = joj::JFloat3(1.0f, -1.0f, 1.0f);
        lightBuffer.specularColor = joj::JFloat4(0.0f, 0.0f, 1.0f, 1.0f);
        lightBuffer.specularPower = 32.0f;
        m_light_cb.update(renderer.get_cmd_list(), lightBuffer);
    }

    {
        CameraBufferType cameraBuffer;
        cameraBuffer.cameraPosition = m_cam.get_pos();
        m_camera_cb.update(renderer.get_cmd_list(), cameraBuffer);
    }
}

void HelloTriangle::draw()
{
    renderer.clear();

    // Configurar o pipeline gráfico
    renderer.get_cmd_list().device_context->IASetInputLayout(gInputLayout);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_vb.bind(renderer.get_cmd_list(), 0, 1, &stride, &offset);
    m_ib.bind(renderer.get_cmd_list(), joj::DataFormat::R32_UINT, offset);
    
    renderer.get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    m_mat_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 0, 1);
    m_light_cb.bind_to_pixel_shader(renderer.get_cmd_list(), 0, 1);
    m_camera_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 1, 1);

    m_tex.bind(renderer.get_cmd_list(), 0, 1);
    renderer.get_cmd_list().device_context->PSSetSamplers(0, 1, &m_sampleState);

    m_shader.bind_vertex_shader(renderer.get_cmd_list());
    m_shader.bind_pixel_shader(renderer.get_cmd_list());

    renderer.get_cmd_list().device_context->DrawIndexed(indexCount, 0, 0);

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();

    m_sampleState->Release();
    gInputLayout->Release();
}

f32 HelloTriangle::get_frametime()
{
#ifdef JOJ_DEBUG_MODE
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter
#endif // JOJ_DEBUG_MODE

    // Current frame time
    frametime = timer.reset();

#ifdef JOJ_DEBUG_MODE
    // Accumulated frametime
    total_time += frametime;

    // Increment frame counter
    frame_count++;

    // Updates FPS indicator in the window every 1000ms (1 second)
    if (total_time >= 1.0f)
    {
        std::stringstream text;		// Text flow for messages
        text << std::fixed;			// Always show the fractional part
        text.precision(3);			// three numbers after comma

        text << "Joj Engine v0.0.1" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << frametime * 1000 << " (ms)";

        window.set_title(text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
#endif // JOJ_DEBUG_MODE

    return frametime;
}