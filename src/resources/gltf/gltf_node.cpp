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

joj::Matrix4x4 joj::compute_local_transform(const GLTFNode& node)
{
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
    DirectX::XMVECTOR q = node.rotation.to_XMVECTOR();
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(q);
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
    DirectX::XMMATRIX SRT = S * R * T;

    Matrix4x4 local_transform;
    local_transform.from_XMMATRIX(SRT);

    return local_transform;
}