#include "joj/resources/mesh.h"

joj::Mesh::Mesh()
{
}

joj::Mesh::~Mesh()
{
}

void joj::Mesh::set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices = vertices;
}

void joj::Mesh::set_indices(const std::vector<u16>& indices)
{
    m_indices = indices;
}

void joj::Mesh::set_submeshes(const std::vector<Submesh>& submeshes)
{
    m_submeshes = submeshes;
}

const std::vector<joj::Vertex::ColorTanPosNormalTex>& joj::Mesh::get_vertex_data() const
{
    return m_vertices;
}

const std::vector<u16>& joj::Mesh::get_index_data() const
{
    return m_indices;
}

const std::vector<joj::Submesh>& joj::Mesh::get_submeshes() const
{
    return m_submeshes;
}

u32 joj::Mesh::get_vertex_count() const
{
    return static_cast<u32>(m_vertices.size());
}

u32 joj::Mesh::get_index_count() const
{
    return static_cast<u32>(m_indices.size());
}

u32 joj::Mesh::get_submesh_count() const
{
    return static_cast<u32>(m_submeshes.size());
}