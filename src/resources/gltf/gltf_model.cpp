#include "joj/resources/gltf/gltf_model.h"

#include "joj/core/logger.h"
#include "joj/resources/mesh.h"
#include <iostream>
#include "joj/core/jmacros.h"
#include <fstream>
#include <filesystem>

joj::GLTFModel::GLTFModel()
{
}

joj::GLTFModel::~GLTFModel()
{
}

void joj::GLTFModel::clear_root_nodes()
{
    m_root_nodes.clear();
}

void joj::GLTFModel::add_gltf_mesh(const GLTFMesh& mesh)
{
    m_gltf_meshes.push_back(mesh);
}

void joj::GLTFModel::add_gltf_node(const GLTFNode& node)
{
    m_gltf_nodes.push_back(node);
}

void joj::GLTFModel::add_root_node(const i32 node_index)
{
    if (node_index < 0 || node_index >= static_cast<i32>(m_gltf_nodes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::add_root_node: index out of bounds: %d", node_index);
        return;
    }

    m_root_nodes.push_back(node_index);
}

void joj::GLTFModel::add_root_node(const std::string& node_name)
{
    for (const auto& node : m_gltf_nodes)
    {
        if (node.name == node_name)
        {
            m_root_nodes.push_back(node.mesh_index);
            return;
        }
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::add_root_node: node not found: %s", node_name.c_str());
}

void joj::GLTFModel::add_aggregated_mesh(const Mesh& mesh)
{
    m_aggregated_meshes.push_back(mesh);
}

void joj::GLTFModel::set_gltf_meshes(const std::vector<GLTFMesh>& meshes)
{
    m_gltf_meshes = meshes;
}

void joj::GLTFModel::set_gltf_nodes(const std::vector<GLTFNode>& nodes)
{
    m_gltf_nodes = nodes;
}

void joj::GLTFModel::set_root_nodes(const std::vector<i32>& root_nodes)
{
    m_root_nodes = root_nodes;
}

void joj::GLTFModel::set_aggregated_meshes(const std::vector<Mesh>& meshes)
{
    m_aggregated_meshes = meshes;
}

const std::vector<joj::Vertex::ColorTanPosNormalTex>& joj::GLTFModel::get_vertices() const
{
    return m_vertices;
}

const std::vector<u16>& joj::GLTFModel::get_indices() const
{
    return m_indices;
}

const std::vector<joj::Submesh>& joj::GLTFModel::get_submeshes() const
{
    return m_submeshes;
}

const i32 joj::GLTFModel::get_vertex_count() const
{
    return static_cast<i32>(m_vertices.size());
}

const i32 joj::GLTFModel::get_index_count() const
{
    return static_cast<i32>(m_indices.size());
}

const i32 joj::GLTFModel::get_submesh_count() const
{
    return static_cast<i32>(m_submeshes.size());
}

const std::vector<joj::GLTFMesh>& joj::GLTFModel::get_gltf_meshes() const
{
    return m_gltf_meshes;
}

const joj::GLTFMesh* joj::GLTFModel::get_gltf_mesh(const i32 index) const
{
    if (index < 0 || index >= static_cast<i32>(m_gltf_meshes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_mesh: index out of bounds: %d", index);
        return nullptr;
    }

    return &m_gltf_meshes[index];
}

const joj::GLTFMesh* joj::GLTFModel::get_gltf_mesh(const std::string& name) const
{
    for (const auto& mesh : m_gltf_meshes)
    {
        if (mesh.name == name)
            return &mesh;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_mesh: mesh not found: %s", name.c_str());
    return nullptr;
}

const std::vector<joj::GLTFNode>& joj::GLTFModel::get_nodes() const
{
    return m_gltf_nodes;
}

const joj::GLTFNode* joj::GLTFModel::get_node(const i32 index) const
{
    if (index < 0 || index >= static_cast<i32>(m_gltf_nodes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_node: index out of bounds: %d", index);
        return nullptr;
    }

    return &m_gltf_nodes[index];
}

const joj::GLTFNode* joj::GLTFModel::get_node(const std::string& name) const
{
    for (const auto& node : m_gltf_nodes)
    {
        if (node.name == name)
            return &node;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_node: node not found: %s", name.c_str());
    return nullptr;
}

const joj::GLTFNode* joj::GLTFModel::get_node_by_mesh(const i32 mesh_index) const
{
    if (mesh_index < 0 || mesh_index >= static_cast<i32>(m_gltf_meshes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_node_by_mesh: index out of bounds: %d", mesh_index);
        return nullptr;
    }

    for (const auto& node : m_gltf_nodes)
    {
        if (node.mesh_index == mesh_index)
            return &node;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_node_by_mesh: node not found for mesh index: %d", mesh_index);
    return nullptr;
}

const joj::GLTFNode* joj::GLTFModel::get_node_by_mesh(const std::string& mesh_name) const
{
    for (const auto& node : m_gltf_nodes)
    {
        if (node.name == mesh_name)
            return &node;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_node_by_mesh: node not found for mesh name: %s", mesh_name.c_str());
    return nullptr;
}

const std::vector<i32>& joj::GLTFModel::get_root_nodes() const
{
    return m_root_nodes;
}

const i32 joj::GLTFModel::get_root_node(const i32 index) const
{
    if (index < 0 || index >= static_cast<i32>(m_root_nodes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_root_node: index out of bounds: %d", index);
        return -1;
    }

    return m_root_nodes[index];
}

const i32 joj::GLTFModel::get_root_node(const std::string& name) const
{
    for (const auto& node : m_gltf_nodes)
    {
        if (node.name == name)
            return node.mesh_index;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_root_node: root node not found: %s", name.c_str());
    return -1;
}

const i32 joj::GLTFModel::get_root_node_by_mesh(const i32 mesh_index) const
{
    if (mesh_index < 0 || mesh_index >= static_cast<i32>(m_gltf_meshes.size()))
    {
        JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_root_node_by_mesh: index out of bounds: %d", mesh_index);
        return -1;
    }

    for (const auto& node : m_gltf_nodes)
    {
        if (node.mesh_index == mesh_index)
            return node.mesh_index;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_root_node_by_mesh: root node not found for mesh index: %d", mesh_index);
    return -1;
}

const i32 joj::GLTFModel::get_root_node_by_mesh(const std::string& mesh_name) const
{
    for (const auto& node : m_gltf_nodes)
    {
        if (node.name == mesh_name)
            return node.mesh_index;
    }

    JOJ_ERROR(ErrorCode::FAILED, "GLTFModel::get_root_node_by_mesh: root node not found for mesh name: %s", mesh_name.c_str());
    return -1;
}

const i32 joj::GLTFModel::get_gltf_meshes_count() const
{
    return static_cast<i32>(m_gltf_meshes.size());
}

const i32 joj::GLTFModel::get_gltf_nodes_count() const
{
    return static_cast<i32>(m_gltf_nodes.size());
}

const i32 joj::GLTFModel::get_root_nodes_count() const
{
    return static_cast<i32>(m_root_nodes.size());
}

const i32 joj::GLTFModel::get_aggregated_meshes_count() const
{
    return static_cast<i32>(m_aggregated_meshes.size());
}

void joj::GLTFModel::print_info() const
{
    std::cout << "==================================================\n";
    std::cout << "GLTFModel Info:\n";
    std::cout << "    Meshes: " << m_gltf_meshes.size() << "\n";
    std::cout << "    Nodes: " << m_gltf_nodes.size() << "\n";
    std::cout << "    Root Nodes: " << m_root_nodes.size() << "\n";
    std::cout << "==================================================\n";
}

void joj::GLTFModel::print_hierarchy() const
{
    std::cout << "==================================================\n";
    std::cout << "GLTFModel Hierarchy:\n";
    std::cout << "==================================================\n";

    for (const auto& node : m_gltf_nodes)
    {
        std::cout << "Node: " << node.name << "\n";
        std::cout << "  Mesh Index: " << node.mesh_index << "\n";
        std::cout << "  Children: ";
        for (const auto& child : node.children)
        {
            std::cout << child << " ";
        }
        std::cout << "\n";
    }

    std::cout << "==================================================\n";
}

void joj::GLTFModel::print_meshes() const
{
    std::cout << "==================================================\n";
    std::cout << "GLTFModel Meshes:\n";
    std::cout << "==================================================\n";

    for (const auto& mesh : m_gltf_meshes)
    {
        std::cout << "Mesh: " << mesh.name << "\n";
        std::cout << "  Primitives: " << mesh.primitives.size() << "\n";
        for (const auto& primitive : mesh.primitives)
        {
            std::cout << "    Primitive: " << primitive.indices_acessor << "\n";
        }
    }

    std::cout << "==================================================\n";
}

void joj::GLTFModel::print_nodes() const
{
    std::cout << "==================================================\n";
    std::cout << "GLTFModel Nodes:\n";
    std::cout << "==================================================\n";

    for (const auto& node : m_gltf_nodes)
    {
        std::cout << "Node: " << node.name << "\n";
        std::cout << "  Mesh Index: " << node.mesh_index << "\n";
        std::cout << "  Children: ";
        for (const auto& child : node.children)
        {
            std::cout << child << " ";
        }
        std::cout << "\n";
    }

    std::cout << "==================================================\n";
}

void joj::GLTFModel::write_data_to_file(const char* filename) const
{
    JOJ_ASSERT(filename != nullptr, "Filename is null.");

    namespace fs = std::filesystem;

    if (fs::exists(filename))
    {
        std::string new_filename = "";
        new_filename += filename;
        new_filename += "-old";

        while (fs::exists(new_filename))
            new_filename += "-old";

        fs::rename(filename, new_filename);
        std::cout << "Renamed existing file to: " << new_filename << std::endl;
    }

    std::ofstream vertices_file(filename);
    if (!vertices_file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    vertices_file << "Vertices Count: " << get_vertex_count() << "\n";
    vertices_file << "Indices Count: " << get_index_count() << "\n";
    vertices_file << "Submeshes Count: " << get_gltf_meshes_count() << "\n";

    // Write GLTF meshes
    i32 mesh_index = 0;
    for (const auto& mesh : get_gltf_meshes())
    {
        vertices_file << "Mesh[" << mesh_index++ << "]: " << mesh.name << "\n";
        vertices_file << "    Primitives: " << mesh.primitives.size() << "\n";

        for (const auto& primitive : mesh.primitives)
        {
            vertices_file << "        Primitive: " << primitive.indices_acessor << "\n";
            vertices_file << "            Position Accessor: " << primitive.position_acessor << "\n";
            vertices_file << "            Normal Accessor: " << primitive.normal_acessor << "\n";
            vertices_file << "            Color Accessor: " << primitive.color_acessor << "\n";
        }
    }

    // Write vertices for each submesh in mesh Model
    i32 submesh_index = 0;
    for (const auto& submesh : get_submeshes())
    {
        vertices_file << "Submesh[" << submesh_index++ << "]: " << submesh.name << "\n";
        vertices_file << "    Vertex Start: " << submesh.vertex_start << "\n";
        vertices_file << "    Vertex Count: " << submesh.vertex_count << "\n";
        vertices_file << "    Index Start: " << submesh.index_start << "\n";
        vertices_file << "    Index Count: " << submesh.index_count << "\n";

        vertices_file << "    Vertices: \n";
        for (i32 i = submesh.vertex_start; i < submesh.vertex_start + submesh.vertex_count; ++i)
        {
            const auto& vertex = get_vertices()[i];
            vertices_file << "        " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
        }

        vertices_file << "    Indices: \n";
        for (i32 i = submesh.index_start; i < submesh.index_start + submesh.index_count; ++i)
        {
            const auto& index = get_indices()[i];
            vertices_file << "        " << index << "\n";
        }

        ++submesh_index;
    }

    // Write vertices
    vertices_file << "Vertices: \n";
    i32 vertex_index = 0;
    for (const auto& vertex : get_vertices())
    {
        vertices_file << "Vertex[" << vertex_index++ << "]: " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << "\n";
    }
    vertices_file << "Indices: \n";
    i32 index_index = 0;
    for (const auto& index : get_indices())
    {
        vertices_file << "Index[" << index_index++ << "]: " << index << "\n";
    }

    vertices_file.close();
}