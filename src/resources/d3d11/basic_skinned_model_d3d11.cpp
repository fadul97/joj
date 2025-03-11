#include "joj/resources/d3d11/basic_skinned_model_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/d3d11/texture2d_data_d3d11.h"
#include "joj/resources/m3d_loader.h"
#include "joj/resources/d3d11/mesh_d3d11.h"
#include "joj/core/jmacros.h"

joj::D3D11BasicSkinnedModel::D3D11BasicSkinnedModel()
    : BasicSkinnedModel()
{
}

joj::D3D11BasicSkinnedModel::~D3D11BasicSkinnedModel()
{
    if (m_mesh != nullptr)
    {
        delete m_mesh;
        m_mesh = nullptr;
    }
}

joj::ErrorCode joj::D3D11BasicSkinnedModel::load_m3d(GraphicsDevice& device,
    CommandList& cmd_list, TextureManager& tex_mgr,
    const std::string& model_filename, const std::wstring& texture_path)
{
    std::vector<M3DMaterial> mats;
    if JOJ_FAILED(M3DLoader::load_M3D_skinned(model_filename, m_vertices,
        m_indices, m_submeshes, mats, m_skinned_data))
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_LOAD_M3D_SKINNED_FAILED,
            "Failed to load Static M3D Model '%s'.", model_filename.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_SKINNED_FAILED;
    }
    
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

    m_submesh_count = static_cast<u32>(mats.size());

    for (u32 i = 0; i < m_submesh_count; ++i)
    {
        m_mat.push_back(mats[i].mat);

        if JOJ_FAILED(tex_mgr.create(device, cmd_list,
            texture_path + mats[i].diffuse_map_name, ImageType::DDS))
        {
            // std::string filepath = texture_path.c_str() + mats[i].diffuse_map_name;
            JOJ_ERROR(ErrorCode::ERR_TEXTURE_2D_CREATION,
                "Failed to create Texture2D for file '%ls'.", mats[i].diffuse_map_name);
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
                "Failed to create Texture2D for file '%ls'.", mats[i].normal_map_name);
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

#endif // JOJ_PLATFORM_WINDOWS