#ifndef _JOJ_D3D11_MESH_H
#define _JOJ_D3D11_MESH_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "resources/mesh.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"

namespace joj
{
    class JOJ_API D3D11Mesh : public Mesh
    {
	public:
		D3D11Mesh();
		~D3D11Mesh();

		ErrorCode set_indices(GraphicsDevice& device, const u16* indices,
			u32 count) override;

		void set_submesh_table(std::vector<SubMesh>& submesh_table) override;

		void draw(CommandList& cmd_list, u32 submesh_id) override;

	protected:
		ErrorCode set_vertices_internal(GraphicsDevice& device, const void* vertices,
			const u32 sizeof_data, const u32 count) override;

	private:
		D3D11VertexBuffer m_vb;
		D3D11IndexBuffer m_ib;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_MESH_H