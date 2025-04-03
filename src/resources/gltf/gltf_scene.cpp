#include "joj/resources/gltf/gltf_scene.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#if JOJ_PLATFORM_WINDOWS
#include "joj/renderer/d3d11/renderer_d3d11.h"
#endif

#include "joj/core/jmacros.h"

joj::GLTFScene::GLTFScene()
{
}

joj::GLTFScene::~GLTFScene()
{
}

void joj::GLTFScene::add_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
}

void joj::GLTFScene::add_indices(const std::vector<u16>& indices)
{
    m_indices.insert(m_indices.end(), indices.begin(), indices.end());
}

void joj::GLTFScene::add_submesh(const Submesh& submesh)
{
    m_submeshes.push_back(submesh);
}

void joj::GLTFScene::write_vertices_to_file(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    i32 count = 0;
    for (const auto& vertex : m_vertices)
    {
        file << "Vertex " << count++ << ": " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
    }
    file.close();
}

void joj::GLTFScene::write_indices_to_file(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    i32 count = 0;
    for (const auto& index : m_indices)
    {
        file << "Index " << count++ << ": " << index << "\n";
    }
    file.close();
}

void joj::GLTFScene::write_vertices_and_indices_to_file(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;
/*
Vertices:
Count: 4145
Indices: 16182
Submeshes: 3
Submeshes:
    Submesh Name: LanternPole_Body
        Vertex Start: 0
        Vertex Count: 926
        Index Start: 0
        Index Count: 2616
Submesh Name[0]: LanternPole_Body
    Vertex Start: 0
    Vertex Count: 926
    => Vertices
        0: 2.081796646118164, -12.278518676757812, 1.741357684135437
        1: 2.081796407699 ....
*/
    file << "Vertices:\n";
    file << "Count: " << m_vertices.size() << "\n";
    file << "Indices: " << m_indices.size() << "\n";
    file << "Submeshes: " << m_submeshes.size() << "\n";
    file << "Submeshes:\n";
    i32 count = 0;
    for (const auto& submesh : m_submeshes)
    {
        file << "Submesh Name[" << count << "]: " << submesh.name << "\n";
        file << "    Vertex Start: " << submesh.vertex_start << "\n";
        file << "    Vertex Count: " << submesh.vertex_count << "\n";
        file << "    Index Start: " << submesh.index_start << "\n";
        file << "    Index Count: " << submesh.index_count << "\n";

        file << "    => Vertices\n";
        for (i32 i = submesh.vertex_start; i < submesh.vertex_start + submesh.vertex_count; ++i)
        {
            const auto& vertex = m_vertices[i];
            // Fix precision to 4 decimal places
            file << "        " << i << ": " << std::fixed << std::setprecision(4) << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
            // file << "        " << i << ": " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
        }
        file << "    => Indices\n";
        file << "    Submesh Name[" << count << "]: " << submesh.name << "\n";
        for (i32 i = submesh.index_start; i < submesh.index_start + submesh.index_count; ++i)
        {
            const auto& index = m_indices[i];
            file << "        " << index << "\n";
        }
        ++count;
    }

    file.close();
}

void joj::GLTFScene::write_submesh_data_to_file(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    i32 count = 0;
    for (const auto& submesh : m_submeshes)
    {
        file << "Submesh " << count++ << ": " << submesh.name << "\n";
        file << "    Vertex Start: " << submesh.vertex_start << "\n";
        file << "    Vertex Count: " << submesh.vertex_count << "\n";
        file << "    Index Start: " << submesh.index_start << "\n";
        file << "    Index Count: " << submesh.index_count << "\n";
    }
    file.close();
}

void joj::GLTFScene::set_name(const std::string& name)
{
    m_name = name;
}

void joj::GLTFScene::set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices = vertices;
}

void joj::GLTFScene::set_indices(const std::vector<u16>& indices)
{
    m_indices = indices;
}

const std::string& joj::GLTFScene::get_name() const
{
    return m_name;
}

const std::vector<joj::Vertex::ColorTanPosNormalTex>& joj::GLTFScene::get_vertex_data() const
{
    return m_vertices;
}

const std::vector<u16>& joj::GLTFScene::get_index_data() const
{
    return m_indices;
}

const i32 joj::GLTFScene::get_vertex_count() const
{
    return static_cast<i32>(m_vertices.size());
}

const i32 joj::GLTFScene::get_index_count() const
{
    return static_cast<i32>(m_indices.size());
}

const i32 joj::GLTFScene::get_submesh_count() const
{
    return static_cast<i32>(m_submeshes.size());
}

void joj::GLTFScene::print_info() const
{
    std::cout << "==================================================\n";
    std::cout << "GLTF Scene: " << m_name << "\n";
    std::cout << "Vertices Count: " << m_vertices.size() << "\n";
    std::cout << "Indices Count: " << m_indices.size() << "\n";
    std::cout << "Submeshes Count: " << m_submeshes.size() << "\n";

    i32 count = 0;
    for (const auto& submesh : m_submeshes)
    {
        std::cout << "Submesh " << count++ << ": " << submesh.name << "\n";
        std::cout << "    Vertex Start: " << submesh.vertex_start << "\n";
        std::cout << "    Vertex Count: " << submesh.vertex_count << "\n";
        std::cout << "    Index Start: " << submesh.index_start << "\n";
        std::cout << "    Index Count: " << submesh.index_count << "\n";
    }
}

void joj::GLTFScene::draw(IRenderer* renderer) const
{
    JOJ_ASSERT(renderer != nullptr, "Renderer is null!");

    for (const auto& submesh : m_submeshes)
    {
        renderer->draw_indexed(submesh.index_count, submesh.index_start, submesh.vertex_start);
    }
}

void joj::GLTFScene::draw_mesh_index(IRenderer* renderer, const u32 submesh) const
{
    JOJ_ASSERT(renderer != nullptr, "Renderer is null!");
    JOJ_ASSERT(submesh < m_submeshes.size(), "Submesh index out of range!");

    const auto& submesh_data = m_submeshes[submesh];
    renderer->draw_indexed(submesh_data.index_count, submesh_data.index_start, submesh_data.vertex_start);
}