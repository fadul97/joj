#include "joj/resources/d3d11/basic_model_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/d3d11/texture2d_data_d3d11.h"
#include "joj/resources/m3d_loader.h"
#include "joj/resources/d3d11/mesh_d3d11.h"
#include "joj/core/jmacros.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "joj/renderer/vertex.h"

joj::D3D11BasicModel::D3D11BasicModel()
    : BasicModel()
{
}

joj::D3D11BasicModel::~D3D11BasicModel()
{
    if (m_mesh != nullptr)
    {
        delete m_mesh;
        m_mesh = nullptr;
    }
}

joj::ErrorCode joj::D3D11BasicModel::load_m3d(GraphicsDevice& device,
    CommandList& cmd_list, TextureManager& tex_mgr,
    const std::string& model_filename, const std::wstring& texture_path)
{
    std::vector<M3DMaterial> mats;
    if JOJ_FAILED(M3DLoader::load_M3D_static(model_filename, m_vertices,
        m_indices, m_submeshes, mats))
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED,
            "Failed to load Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED;
    }

    m_submesh_count = static_cast<u32>(mats.size());

    m_mesh = new D3D11Mesh();

    if JOJ_FAILED(m_mesh->set_vertices(device, &m_vertices[0],
        static_cast<u32>(m_vertices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_VERTICES_FAILED,
            "Failed to set vertices for Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED;
    }

    if JOJ_FAILED(m_mesh->set_indices(device, &m_indices[0],
        static_cast<u32>(m_indices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED,
            "Failed to set indices for Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED;
    }

    m_mesh->set_submesh_table(m_submeshes);

    for (u32 i = 0; i < m_submesh_count; ++i)
    {
        m_mat.push_back(mats[i].mat);

        if JOJ_FAILED(tex_mgr.create(device, cmd_list,
            texture_path + mats[i].diffuse_map_name, ImageType::DDS))
        {
            // std::string filepath = texture_path.c_str() + mats[i].diffuse_map_name;
            JOJ_ERROR(ErrorCode::ERR_TEXTURE_2D_CREATION,
                "Failed to create Texture2D for file '%s'.", mats[i].diffuse_map_name);
        }
        else
        {
            TextureData2D* diffuse_map_SRV = tex_mgr.get_texture(texture_path + mats[i].diffuse_map_name);
            if (diffuse_map_SRV->srv != nullptr)
            {
                m_diffuse_map_SRV.push_back(diffuse_map_SRV);
            }
        }

        if JOJ_FAILED(tex_mgr.create(device, cmd_list,
            texture_path + mats[i].normal_map_name, ImageType::DDS))
        {
            // std::string filepath = texture_path.c_str() + mats[i].normal_map_name;
            JOJ_ERROR(ErrorCode::ERR_TEXTURE_2D_CREATION,
                "Failed to create Texture2D for file '%s'.", mats[i].normal_map_name);
        }
        else
        {
            TextureData2D* normal_map_SRV = tex_mgr.get_texture(texture_path + mats[i].diffuse_map_name);
            if (normal_map_SRV->srv != nullptr)
            {
                m_normal_map_SRV.push_back(normal_map_SRV);
            }
        }
    }

    return ErrorCode::OK;
}

joj::ErrorCode joj::D3D11BasicModel::load_obj(GraphicsDevice& device, CommandList& cmd_list,
    const std::string& model_filename)
{
    std::ifstream fin(model_filename);
    if (!fin.is_open())
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_OPENED_FAILED,
            "Failed to open file '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_OPENED_FAILED;
    }

    std::vector<JFloat3> temp_positions;
    std::vector<JFloat3> temp_normals;
    std::vector<JFloat2> temp_texcoords;

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
            JFloat3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        // Normals
        else if (prefix == "vn")
        {
            JFloat3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Tex Coordinates
        else if (prefix == "vt")
        {
            JFloat2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        // Faces
        else if (prefix == "f")
        {
            u32 posIdx[3], texIdx[3], normIdx[3];

            for (i32 i = 0; i < 3; ++i)
            {
                std::string vertexData;
                while (iss >> vertexData)
                {
                    std::istringstream vertexStream(vertexData);
                    std::string v, t, n;
                    u32 posIndex = 0, texIndex = 0, normIndex = 0;

                    std::getline(vertexStream, v, '/');
                    if (!v.empty()) posIndex = std::stoi(v) - 1;

                    if (std::getline(vertexStream, t, '/'))
                    {
                        if (!t.empty()) texIndex = std::stoi(t) - 1;
                    }

                    if (std::getline(vertexStream, n))
                    {
                        if (!n.empty()) normIndex = std::stoi(n) - 1;
                    }

                    Vertex::PosNormalTexTan vertex = {};
                    if (posIndex < temp_positions.size()) vertex.pos = temp_positions[posIndex];
                    if (texIndex < temp_texcoords.size()) vertex.tex = temp_texcoords[texIndex];
                    if (normIndex < temp_normals.size()) vertex.normal = temp_normals[normIndex];

                    auto it = std::find(m_vertices.begin(), m_vertices.end(), vertex);
                    if (it != m_vertices.end())
                    {
                        m_indices.push_back(static_cast<u32>(std::distance(m_vertices.begin(), it)));
                    }
                    else
                    {
                        m_vertices.push_back(vertex);
                        m_indices.push_back(static_cast<u32>(m_vertices.size() - 1));
                    }
                }
            }
        }
    }

    // FIXME: Hardcoded
    m_submesh_count = static_cast<u32>(1);

    m_mesh = new D3D11Mesh();

    if JOJ_FAILED(m_mesh->set_vertices(device, &m_vertices[0],
        static_cast<u32>(m_vertices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_VERTICES_FAILED,
            "Failed to set vertices for Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED;
    }

    if JOJ_FAILED(m_mesh->set_indices(device, &m_indices[0],
        static_cast<u32>(m_indices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED,
            "Failed to set indices for Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED;
    }

    // FIXME: Hardcoded
    m_submeshes.resize(m_submesh_count);
    m_submeshes[0].id = 0;
    m_submeshes[0].vertex_start = 0;
    m_submeshes[0].vertex_count = 8;
    m_submeshes[0].face_start = 0;
    m_submeshes[0].face_count = 36;

    m_mesh->set_submesh_table(m_submeshes);

    return ErrorCode::OK;
}

joj::ErrorCode joj::D3D11BasicModel::load_jsf(GraphicsDevice& device, CommandList& cmd_list,
    const std::string& model_filename)
{
    std::ifstream fin(model_filename);
    if (!fin.is_open())
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_OPENED_FAILED,
            "Failed to open file '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_OPENED_FAILED;
    }

    u32 num_vertices = 0;
    u32 num_triangles = 0;
    u32 num_submeshes;

    std::string ignore;
    fin >> ignore; // File header text
    fin >> ignore >> num_vertices;
    fin >> ignore >> num_triangles;
    fin >> ignore >> num_submeshes;

    m_submeshes.resize(num_submeshes);

    fin >> ignore; // Submesh header text
    for (u32 i = 0; i < num_submeshes; ++i)
    {
        fin >> ignore >> m_submeshes[i].id;
        fin >> ignore >> m_submeshes[i].vertex_start;
        fin >> ignore >> m_submeshes[i].vertex_count;
        fin >> ignore >> m_submeshes[i].face_start;
        fin >> ignore >> m_submeshes[i].face_count;
    }

    fin >> ignore; // Vertices header
    m_vertices.resize(num_vertices);
    for (u32 i = 0; i < num_vertices; ++i)
    {
        Vertex::PosNormalTexTan v;
        fin >> ignore >> v.pos.x >> v.pos.y >> v.pos.z;
        fin >> ignore >> v.normal.x >> v.normal.y >> v.normal.z;
        fin >> ignore >> v.tex.x >> v.tex.y;
    }

    fin >> ignore; // Indices header
    m_indices.resize(num_triangles * 3);
    for (u32 i = 0; i < num_triangles; ++i)
    {
        fin >> ignore >> ignore >> m_indices[i * 3 + 0] >> m_indices[i * 3 + 1] >> m_indices[i * 3 + 2];
    }

    m_submesh_count = static_cast<u32>(1);

    m_mesh = new D3D11Mesh();

    if JOJ_FAILED(m_mesh->set_vertices(device, &m_vertices[0],
        static_cast<u32>(m_vertices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_VERTICES_FAILED,
            "Failed to set vertices for Static Joj Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED;
    }

    if JOJ_FAILED(m_mesh->set_indices(device, &m_indices[0],
        static_cast<u32>(m_indices.size())))
    {
        JOJ_ERROR(ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED,
            "Failed to set indices for Static Joj Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_BASIC_MODEL_SET_INDICES_FAILED;
    }

    m_mesh->set_submesh_table(m_submeshes);

    return ErrorCode::OK;
}


#endif // JOJ_PLATFORM_WINDOWS