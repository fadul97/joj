#include "joj/resources/scene_node.h"

joj::SceneNode::SceneNode()
    : m_name(""), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_mesh(nullptr), m_parent(nullptr)
{
    m_local_matrix = Matrix4x4::identity();
}

joj::SceneNode::SceneNode(const std::string& name)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_mesh(nullptr), m_parent(nullptr)
{
    m_local_matrix = Matrix4x4::identity();
}

joj::SceneNode::SceneNode(const std::string& name, const i32 parent_index)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_mesh(nullptr), m_parent(nullptr)
{
    m_local_matrix = Matrix4x4::identity();
}

joj::SceneNode::~SceneNode()
{
}

void joj::SceneNode::set_name(const char* name)
{
    m_name = name;
}

void joj::SceneNode::set_position(const Vector3& pos)
{
    m_position = pos;
}

void joj::SceneNode::set_rotation(const Vector4& rot)
{
    m_rotation = rot;
}

void joj::SceneNode::set_scale(const Vector3& scl)
{
    m_scale = scl;
}

void joj::SceneNode::set_mesh(const Mesh* mesh)
{
    m_mesh = mesh;
}

void joj::SceneNode::add_child(const SceneNode* child_node)
{
    m_children.push_back(child_node);
}

void joj::SceneNode::set_parent(const SceneNode* parent_node)
{
    m_parent = parent_node;
}

std::string joj::SceneNode::get_name() const
{
    return m_name;
}

joj::Vector3 joj::SceneNode::get_position() const
{
    return m_position;
}

joj::Vector4 joj::SceneNode::get_rotation() const
{
    return m_rotation;
}

joj::Vector3 joj::SceneNode::get_scale() const
{
    return m_scale;
}

const joj::Mesh* joj::SceneNode::get_mesh() const
{
    return m_mesh;
}

const std::vector<const joj::SceneNode*>& joj::SceneNode::get_children() const
{
    return m_children;
}

const joj::SceneNode* joj::SceneNode::get_parent() const
{
    return m_parent;
}