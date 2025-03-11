#ifndef _JOJ_BASIC_MODEL_H
#define _JOJ_BASIC_MODEL_H

#include "joj/core/defines.h"

#include "joj/renderer/renderer.h"
#include "joj/renderer/texture_manager.h"
#include <string>
#include <vector>
#include "joj/renderer/texture2d_data.h"
#include "joj/systems/material/material.h"
#include "joj/renderer/vertex.h"
#include "submesh.h"
#include "mesh.h"
#include "joj/math/jmath.h"

namespace joj
{
    class JOJ_API BasicModel
    {
    public:
        BasicModel();
        virtual ~BasicModel();

        virtual ErrorCode load_obj(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_filename) = 0;

        virtual ErrorCode load_m3d(GraphicsDevice& device, CommandList& cmd_list,
            TextureManager& tex_mgr, const std::string& model_filename,
            const std::wstring& texture_path) = 0;

        virtual ErrorCode load_jsf(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_filename) = 0;

        u32 get_submesh_count() const;

        std::vector<Material>& get_mat();
        std::vector<TextureData2D*>& get_diffuse_map_SRV();
        std::vector<TextureData2D*>& get_normal_map_SRV();

        Mesh* get_mesh();

    protected:
        u32 m_submesh_count;

        std::vector<Material> m_mat;
        std::vector<TextureData2D*> m_diffuse_map_SRV;
        std::vector<TextureData2D*> m_normal_map_SRV;

        // Keep CPU copies of the mesh data to read from.
        std::vector<Vertex::PosNormalTexTan> m_vertices;
        std::vector<u16> m_indices;
        std::vector<SubMesh> m_submeshes;

        Mesh* m_mesh;
    };

    inline u32 BasicModel::get_submesh_count() const
    { return m_submesh_count; }

    inline std::vector<Material>& BasicModel::get_mat()
    { return m_mat; }

    inline std::vector<TextureData2D*>& BasicModel::get_diffuse_map_SRV()
    { return m_diffuse_map_SRV; }

    inline std::vector<TextureData2D*>& BasicModel::get_normal_map_SRV()
    { return m_normal_map_SRV; }

    inline Mesh* BasicModel::get_mesh()
    { return m_mesh; }

    struct BasicModelInstance
    {
        BasicModel* model;
        JFloat4x4 world;
    };
}

#endif // _JOJ_BASIC_MODEL_H