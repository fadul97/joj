#include "resources/mesh.h"

joj::Mesh::Mesh()
    : m_position{ 0.0f, 0.0f, 0.0f }, m_type(MeshType::None),
    m_vertex_count{0}, m_index_count{0}
{
}

joj::Mesh::Mesh(const std::string& filename, MeshType type)
    : m_position{ 0.0f, 0.0f, 0.0f }, m_type(type),
    m_vertex_count{ 0 }, m_index_count{ 0 }
{
}

joj::Mesh::~Mesh()
{
}