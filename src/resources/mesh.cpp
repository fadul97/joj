#include "joj/resources/mesh.h"

joj::Mesh::Mesh()
    : m_vertex_stride{ 0 },
    m_index_format{ DataFormat::R16_UINT }
{
}

joj::Mesh::~Mesh()
{
}