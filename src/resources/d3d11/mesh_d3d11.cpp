#include "joj/resources/d3d11/mesh_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/jmacros.h"
#include "joj/core/logger.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"

joj::D3D11Mesh::D3D11Mesh()
	: Mesh()
{
	m_vb = D3D11VertexBuffer();
	m_ib = D3D11IndexBuffer();
}

joj::D3D11Mesh::~D3D11Mesh()
{
}

joj::ErrorCode joj::D3D11Mesh::set_indices(GraphicsDevice& device,
	const u16* indices, u32 count)
{
	// Always u16 type indices
	m_ib.setup(sizeof(u16) * count, indices);

	if JOJ_FAILED(m_ib.create(device))
	{
		JOJ_ERROR(ErrorCode::ERR_INDEX_BUFFER_D3D11_CREATION,
			"Failed to create D3D11 Index Buffer for D3D11Mesh.");
		return ErrorCode::ERR_INDEX_BUFFER_D3D11_CREATION;
	}

	return ErrorCode::OK;
}

void joj::D3D11Mesh::set_submesh_table(std::vector<SubMesh>& submesh_table)
{
	m_submesh_table = submesh_table;
}

void joj::D3D11Mesh::draw(CommandList& cmd_list, u32 submesh_id)
{
	u32 offset = 0;

	m_vb.bind(cmd_list, 0, 1, &m_vertex_stride, &offset);
	m_ib.bind(cmd_list, m_index_format, offset);

	cmd_list.device_context->DrawIndexed(
		m_submesh_table[submesh_id].face_count * 3,
		m_submesh_table[submesh_id].face_start * 3,
		0
	);
}

joj::ErrorCode joj::D3D11Mesh::set_vertices_internal(GraphicsDevice& device,
	const void* vertices, const u32 sizeof_data, const u32 count)
{
	if (m_vb.is_filled())
		m_vb.cleanup();

	m_vb.setup(BufferUsage::Immutable, CPUAccessType::None, sizeof_data * count, vertices);

	if JOJ_FAILED(m_vb.create(device))
	{
		JOJ_ERROR(ErrorCode::ERR_VERTEX_BUFFER_D3D11_CREATION,
			"Failed to create D3D11 Vertex Buffer for D3D11Mesh.");
		return ErrorCode::ERR_VERTEX_BUFFER_D3D11_CREATION;
	}

	return ErrorCode::OK;
}

#endif // JOJ_PLATFORM_WINDOWS