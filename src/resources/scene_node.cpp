#include "joj/resources/scene_node.h"

joj::SceneNode::SceneNode()
    : m_name(""), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(-1)
{
}

joj::SceneNode::SceneNode(const std::string& name)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(-1)
{
}

joj::SceneNode::SceneNode(const std::string& name, const i32 parent_index)
    : m_name(name), m_position(Vector3(0.0f, 0.0f, 0.0f))
    , m_rotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f))
    , m_children_indices(), m_parent_index(parent_index)
{
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

void joj::SceneNode::add_child(const i32& child_index)
{
    m_children_indices.push_back(child_index);
}

void joj::SceneNode::set_parent(const i32& parent_index)
{
    m_parent_index = parent_index;
}