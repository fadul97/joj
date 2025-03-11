#ifndef _JOJ_D3D11_MESH_GEOMETRY_OLD_H
#define _JOJ_D3D11_MESH_GEOMETRY_OLD_H

#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "resources/mesh_old.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"
#include "renderer/d3d11/constant_buffer_d3d11.h"
#include "renderer/d3d11/shader_d3d11.h"

namespace joj
{
    class JAPI D3D11MeshGeometryOld : public MeshGeometryOld
    {
    public:
        D3D11MeshGeometryOld();
        D3D11MeshGeometryOld(const std::string& filename, MeshGeometryType type);
        ~D3D11MeshGeometryOld();

        Shader& get_shader() override;

        void setup(GraphicsDevice& device) override;

        void update(const JFloat4x4 view, const JFloat4x4 proj, const f32 dt) override;

        void draw(GraphicsDevice& device, CommandList& cmd_list) override;

    private:
        D3D11VertexBuffer m_vb;
        D3D11IndexBuffer m_ib;
        D3D11ConstantBuffer m_cb;
        D3D11Shader m_shader;
        ConstantBuffer m_cb_data;

        b8 load_OBJ(const std::string& filename, std::vector<VertexOLD>& vertices,
            std::vector<u32>& indices, u32& vertex_count, u32& index_count);
    };

}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_MESH_GEOMETRY_OLD_H