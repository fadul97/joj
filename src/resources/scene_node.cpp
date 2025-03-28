#include "joj/resources/scene_node.h"

joj::SceneNode::SceneNode()
    : m_name(""), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(-1), m_mesh(-1), m_skin(-1)
{
}

joj::SceneNode::SceneNode(const std::string& name)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(-1), m_mesh(-1), m_skin(-1)
{
}

joj::SceneNode::SceneNode(const std::string& name, const i32 parent_index)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(parent_index), m_mesh(-1), m_skin(-1)
{
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

void joj::SceneNode::set_mesh(const i32 mesh_index)
{
    m_mesh = mesh_index;
}

void joj::SceneNode::set_skin(const i32 skin_index)
{
    m_skin = skin_index;
}

void joj::SceneNode::add_child(const i32& child_index)
{
    m_children_indices.push_back(child_index);
}

void joj::SceneNode::set_parent(const i32& parent_index)
{
    m_parent_index = parent_index;
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

i32 joj::SceneNode::get_mesh() const
{
    return m_mesh;
}

i32 joj::SceneNode::get_skin() const
{
    return m_skin;
}

std::vector<i32> joj::SceneNode::get_children_indices() const
{
    return m_children_indices;
}

i32 joj::SceneNode::get_parent_index() const
{
    return m_parent_index;
}