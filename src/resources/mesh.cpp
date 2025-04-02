#include "joj/resources/mesh.h"

#include <iostream>

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

void joj::Mesh::print_vertices() const
{
    std::cout << "========== Mesh Vertices ==========" << std::endl;
    std::cout << "Vertex Count: " << get_vertex_count() << std::endl;
    // Print vertices of each submesh
    i32 i = 0;
    for (const auto& submesh : m_submeshes)
    {
        std::cout << "Submesh Name[" << i << "]: " << submesh.name << std::endl;
        std::cout << "    Vertex Start: " << submesh.vertex_start << std::endl;
        std::cout << "    Vertex Count: " << submesh.vertex_count << std::endl;
        std::cout << "    => Vertices\n";
        for (u32 j = 0; j < submesh.vertex_count; ++j)
        {
            const auto& vertex = m_vertices[submesh.vertex_start + j];
            std::cout << "        " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
        }
        std::cout << "]" << std::endl;
        ++i;
    }

    std::cout << "===================================" << std::endl;
}

void joj::Mesh::print_indices() const
{
    std::cout << "========== Mesh Indices ==========" << std::endl;
    std::cout << "Index Count: " << get_index_count() << std::endl;
    // Print indices of each submesh
    i32 i = 0;
    for (const auto& submesh : m_submeshes)
    {
        std::cout << "Submesh Name[" << i << "]: " << submesh.name << std::endl;
        std::cout << "    Index Start: " << submesh.index_start << std::endl;
        std::cout << "    Index Count: " << submesh.index_count << std::endl;
        std::cout << "    => Indices\n";
        for (u32 j = 0; j < submesh.index_count; ++j)
        {
            const auto& index = m_indices[submesh.index_start + j];
            std::cout << "        " << index << "\n";
        }
        ++i;
    }
    std::cout << "===================================" << std::endl;
}

void joj::Mesh::print_info() const
{
    std::cout << "========== Mesh Info ==========" << std::endl;
    std::cout << "Vertex Count: " << get_vertex_count() << std::endl;
    std::cout << "Index Count: " << get_index_count() << std::endl;
    std::cout << "Submesh Count: " << get_submesh_count() << std::endl;
    print_submesh_info();
    std::cout << "===============================" << std::endl;
}

void joj::Mesh::print_submesh_info() const
{
    for (const auto& submesh : m_submeshes)
    {
        std::cout << "Submesh Name: " << submesh.name << std::endl;
        std::cout << "        Vertex Start: " << submesh.vertex_start << std::endl;
        std::cout << "        Vertex Count: " << submesh.vertex_count << std::endl;
        std::cout << "        Index Start: " << submesh.index_start << std::endl;
        std::cout << "        Index Count: " << submesh.index_count << std::endl;
    }
}