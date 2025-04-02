#include "joj/resources/gltf/gltf_model.h"

#include "joj/core/logger.h"
#include "joj/resources/mesh.h"
#include <iostream>

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