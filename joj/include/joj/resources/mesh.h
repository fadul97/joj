#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
#include <vector>
#include "submesh.h"
#include "renderer/data_format.h"
#include "error_code.h"

namespace joj
{
	class JAPI Mesh
	{
	public:
		Mesh();
		virtual ~Mesh();

		template <typename VertexType>
		ErrorCode set_vertices(GraphicsDevice& device, const VertexType* vertices,
			u32 count);

		virtual ErrorCode set_indices(GraphicsDevice& device, const u16* indices,
			u32 count) = 0;

		virtual void set_submesh_table(std::vector<SubMesh>& submesh_table) = 0;

		virtual void draw(CommandList& cmd_list, u32 submesh_id) = 0;

	protected:
		virtual ErrorCode set_vertices_internal(GraphicsDevice& device, const void* vertices,
			const u32 sizeof_data, const u32 count) = 0;

		DataFormat m_index_format;
		u32 m_vertex_stride;
		std::vector<SubMesh> m_submesh_table;
	};

	template <typename VertexType>
	inline ErrorCode Mesh::set_vertices(GraphicsDevice& device,
		const VertexType* vertices, u32 count)
	{
		// static_assert(sizeof(T) <= max_buffer_size, "Data size exceeds constant buffer limits.");
		m_vertex_stride = sizeof(VertexType);
		return set_vertices_internal(device, vertices, sizeof(VertexType), count);
	}
}

#endif // _JOJ_MESH_H