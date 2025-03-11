#ifndef _JOJ_BASIC_SKINNED_MODEL_H
#define _JOJ_BASIC_SKINNED_MODEL_H

#include "core/defines.h"

#include "renderer/renderer.h"
#include "renderer/texture_manager.h"
#include <string>
#include <vector>
#include "renderer/texture2d_data.h"
#include "systems/material/material.h"
#include "renderer/vertex.h"
#include "submesh.h"
#include "mesh.h"
#include "math/jmath.h"
#include "resources/skinned_data.h"

namespace joj
{
    class JOJ_API BasicSkinnedModel
    {
    public:
        BasicSkinnedModel();
        virtual ~BasicSkinnedModel();

        virtual ErrorCode load_m3d(GraphicsDevice& device, CommandList& cmd_list,
            TextureManager& tex_mgr, const std::string& model_filename,
            const std::wstring& texture_path) = 0;

        u32 get_submesh_count() const;

        std::vector<Material>& get_mat();
        std::vector<TextureData2D*>& get_diffuse_map_SRV();
        std::vector<TextureData2D*>& get_normal_map_SRV();

        Mesh* get_mesh();
        SkinnedData& get_skinned_data();

    protected:
        u32 m_submesh_count;

        std::vector<Material> m_mat;
        std::vector<TextureData2D*> m_diffuse_map_SRV;
        std::vector<TextureData2D*> m_normal_map_SRV;

        // Keep CPU copies of the mesh data to read from.
        std::vector<Vertex::PosNormalTexTanSkinned> m_vertices;
        std::vector<u16> m_indices;
        std::vector<SubMesh> m_submeshes;

        Mesh* m_mesh;
        SkinnedData m_skinned_data;
    };

    inline u32 BasicSkinnedModel::get_submesh_count() const
    { return m_submesh_count; }

    inline std::vector<Material>& BasicSkinnedModel::get_mat()
    { return m_mat; }

    inline std::vector<TextureData2D*>& BasicSkinnedModel::get_diffuse_map_SRV()
    { return m_diffuse_map_SRV; }

    inline std::vector<TextureData2D*>& BasicSkinnedModel::get_normal_map_SRV()
    { return m_normal_map_SRV; }

    inline Mesh* BasicSkinnedModel::get_mesh()
    { return m_mesh; }

    inline SkinnedData& BasicSkinnedModel::get_skinned_data()
    { return m_skinned_data; }

    struct SkinnedModelInstance
    {
        SkinnedModelInstance();
        ~SkinnedModelInstance();

        BasicSkinnedModel* model;
        f32 time_pos;
        std::string clip_name;
        JFloat4x4 world;
        std::vector<JFloat4x4> final_transforms;

        void update(f32 dt);
    };
}

#endif // _JOJ_BASIC_SKINNED_MODEL_H