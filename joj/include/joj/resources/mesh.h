#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
#include <vector>
#include "submesh.h"

namespace joj
{
	class JAPI Mesh
	{
	public:
		Mesh();
		virtual ~Mesh();

		template <typename VertexType>
		void set_vertices(GraphicsDevice& device, const VertexType* vertices,
			u32 count);

		virtual void set_indices(GraphicsDevice& device, const u16* indices,
			u32 count) = 0;

		virtual void set_submesh_table(std::vector<SubMesh>& submesh_table) = 0;

		virtual void draw(CommandList& cmd_list, u32 submesh_id) = 0;

	protected:
		virtual void set_vertices_internal(GraphicsDevice& device, const void* data,
			const u32 sizeof_data, const u32 count) = 0;
	};

	template <typename VertexType>
	inline void Mesh::set_vertices(GraphicsDevice& device,
		const VertexType* vertices, u32 count)
	{
		// static_assert(sizeof(T) <= max_buffer_size, "Data size exceeds constant buffer limits.");
		set_vertices_internal(device, &vertices, sizeof(VertexType), count);
	}
}

#endif // _JOJ_MESH_H