#include "joj/resources/mesh.h"

#include "joj/core/logger.h"

#if JOJ_PLATFORM_WINDOWS
#include "joj/renderer/d3d11/renderer_d3d11.h"
#endif

#include "joj/core/jmacros.h"

joj::Mesh::Mesh()
{
}

joj::Mesh::~Mesh()
{
}

void joj::Mesh::add_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
}

void joj::Mesh::add_indices(const std::vector<u32>& indices)
{
    m_indices.insert(m_indices.end(), indices.begin(), indices.end());
}

void joj::Mesh::add_submesh(const Submesh& submesh)
{
    m_submeshes.push_back(submesh);
}

void joj::Mesh::set_name(const char* name)
{
    m_name = name;
}

void joj::Mesh::set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices = vertices;
}

void joj::Mesh::set_indices(const std::vector<u32>& indices)
{
    m_indices = indices;
}

void joj::Mesh::set_submeshes(const std::vector<Submesh>& submeshes)
{
    m_submeshes = submeshes;
}

const std::string& joj::Mesh::get_name() const
{
    return m_name;
}

const std::vector<joj::Vertex::ColorTanPosNormalTex>& joj::Mesh::get_vertex_data() const
{
    return m_vertices;
}

const std::vector<u32>& joj::Mesh::get_index_data() const
{
    return m_indices;
}

const std::vector<joj::Submesh>& joj::Mesh::get_submeshes() const
{
    return m_submeshes;
}

const i32 joj::Mesh::get_vertex_count() const
{
    return static_cast<i32>(m_vertices.size());
}

const i32 joj::Mesh::get_index_count() const
{
    return static_cast<i32>(m_indices.size());
}

const i32 joj::Mesh::get_submesh_count() const
{
    return static_cast<i32>(m_submeshes.size());
}

void joj::Mesh::draw(IRenderer* renderer) const
{
    JOJ_ASSERT(renderer != nullptr, "Renderer is null!");

    for (const auto& submesh : m_submeshes)
    {
#if JOJ_PLATFORM_WINDOWS
        renderer->draw_indexed(submesh.index_count, submesh.index_start, submesh.vertex_start);
#endif
    }
}

void joj::Mesh::draw_mesh_index(IRenderer* renderer, const u32 submesh_index) const
{
    JOJ_ASSERT(renderer != nullptr, "Renderer is null!");
    JOJ_ASSERT(submesh_index < m_submeshes.size(), "Submesh index out of range!");

    const auto& submesh = m_submeshes[submesh_index];
#if JOJ_PLATFORM_WINDOWS
    if (submesh_index == 4)
    {
        renderer->draw_indexed(submesh.index_count, submesh.index_start, submesh.vertex_start);
        return;
    }
    renderer->draw_indexed(submesh.index_count, submesh.index_start, submesh.vertex_start);
#endif
}