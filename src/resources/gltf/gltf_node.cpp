#include "joj/resources/gltf/gltf_node.h"

joj::GLTFNode::GLTFNode()
    : camera_index(-1), skin_index(-1), mesh_index(-1), scale(1.0f, 1.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f)
{
    translation = Vector3(0.0f, 0.0f, 0.0f);
    matrix = Matrix4x4::identity();
}

joj::GLTFNode::~GLTFNode()
{
}

b8 joj::is_aggregator_node(const GLTFNode& node)
{
    return (node.children.size() > 0 && node.mesh_index == -1);
}