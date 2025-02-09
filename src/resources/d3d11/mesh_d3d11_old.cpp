#include "resources/d3d11/mesh_d3d11_old.h"

#if JPLATFORM_WINDOWS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "core/logger.h"
#include "renderer/d3d11/renderer_d3d11.h"
#include "renderer/d3d11/shader_library_d3d11.h"
#include "core/jmacros.h"

joj::D3D11MeshGeometryOld::D3D11MeshGeometryOld()
    : MeshGeometryOld()
{
    m_cb_data = {};
}

joj::D3D11MeshGeometryOld::D3D11MeshGeometryOld(const std::string& filename, MeshGeometryType type)
    : MeshGeometryOld(filename, type)
{
    if (!load_OBJ(filename, m_vertices, m_indices, m_vertex_count, m_index_count))
    {
        JERROR(ErrorCode::FAILED, "Failed to load OBJ model '%s'.", filename);
        m_vertex_count = 0;
        m_index_count = 0;
        m_cb_data = {};
        return;
    }

    m_cb_data = {};
}

joj::D3D11MeshGeometryOld::~D3D11MeshGeometryOld()
{
}

joj::Shader& joj::D3D11MeshGeometryOld::get_shader()
{
    return m_shader;
}

void joj::D3D11MeshGeometryOld::setup(GraphicsDevice& device)
{
    m_vb.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(VertexOLD) * m_vertex_count, m_vertices.data());
    JOJ_LOG_IF_FAIL(m_vb.create(device));

    m_ib.setup(sizeof(u32) * m_index_count, m_indices.data());
    JOJ_LOG_IF_FAIL(m_ib.create(device));

    m_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
    JOJ_LOG_IF_FAIL(m_cb.create(device));

    m_shader.compile_vertex_shader(D3D11ShaderLibrary::VertexShaderCode,
        "main", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(device));

    m_shader.compile_pixel_shader(D3D11ShaderLibrary::PixelShaderCode,
        "main", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(device));

    JDEBUG("Setup D3D11 Mesh!");
}

void joj::D3D11MeshGeometryOld::update(const JFloat4x4 view, const JFloat4x4 proj, const f32 dt)
{
    static f32 rotation = 0.0f;
    // Update the rotation variable each frame.
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
    {
        rotation += 360.0f;
    }

    {
        using namespace DirectX;

        joj::JMatrix4x4 W = XMMatrixRotationY(rotation); // joj::matrix4x4_identity();
        W = W * XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
        joj::JMatrix4x4 V = XMLoadFloat4x4(&view);
        joj::JMatrix4x4 P = XMLoadFloat4x4(&proj);
        joj::JMatrix4x4 WVP = W * V * P;

        m_cb_data = {};
        XMStoreFloat4x4(&m_cb_data.wvp, XMMatrixTranspose(WVP));
        XMStoreFloat4x4(&m_cb_data.worldMatrix, XMMatrixTranspose(W));
        XMStoreFloat4x4(&m_cb_data.viewMatrix, XMMatrixTranspose(V));
        XMStoreFloat4x4(&m_cb_data.projectionMatrix, XMMatrixTranspose(P));
    }
}

void joj::D3D11MeshGeometryOld::draw(GraphicsDevice& device, CommandList& cmd_list)
{
    UINT stride = sizeof(VertexOLD);
    UINT offset = 0;

    // Bind Vertex Buffer
    m_vb.bind(cmd_list, 0, 1, & stride, & offset);

    // Bind Index Buffer
    m_ib.bind(cmd_list, joj::DataFormat::R32_UINT, offset);

    m_cb.update(cmd_list, m_cb_data);

    // Configurar Constant Buffers e Texturas
    m_cb.bind_to_vertex_shader(cmd_list, 0, 1);

    // Bind Shaders
    m_shader.bind_vertex_shader(cmd_list);
    m_shader.bind_pixel_shader(cmd_list);

    // Desenhar índices
    cmd_list.device_context->DrawIndexed(m_index_count, 0, 0);
}

namespace joj
{
    bool operator==(const VertexOLD& lhs, const VertexOLD& rhs)
    {
        return lhs.position.x == rhs.position.x &&
            lhs.position.y == rhs.position.y &&
            lhs.position.z == rhs.position.z &&
            lhs.texture.x == rhs.texture.x &&
            lhs.texture.y == rhs.texture.y &&
            lhs.normal.x == rhs.normal.x &&
            lhs.normal.y == rhs.normal.y &&
            lhs.normal.z == rhs.normal.z;
    }
}

b8 joj::D3D11MeshGeometryOld::load_OBJ(const std::string& filename, std::vector<VertexOLD>& vertices,
    std::vector<u32>& indices, u32& vertex_count, u32& index_count)
{
    std::vector<joj::JFloat3> positions;
    std::vector<joj::JFloat2> tex_coords;
    std::vector<joj::JFloat3> normals;
    std::vector<VertexOLD> vertex_list;
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

                VertexOLD vertex = {};
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
    vertex_count = static_cast<u32>(vertex_list.size());
    index_count = static_cast<u32>(index_list.size());

    vertices.insert(vertices.end(), vertex_list.begin(), vertex_list.end());
    indices.insert(indices.end(), index_list.begin(), index_list.end());

    return true;
}

#endif // JPLATFORM_WINDOWS