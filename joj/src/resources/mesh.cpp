#include "resources/mesh.h"

joj::MeshGeometryOld::MeshGeometryOld()
    : m_position{ 0.0f, 0.0f, 0.0f }, m_type(MeshGeometryType::None),
    m_vertex_count{0}, m_index_count{0}
{
}

joj::MeshGeometryOld::MeshGeometryOld(const std::string& filename, MeshGeometryType type)
    : m_position{ 0.0f, 0.0f, 0.0f }, m_type(type),
    m_vertex_count{ 0 }, m_index_count{ 0 }
{
}

joj::MeshGeometryOld::~MeshGeometryOld()
{
}