#include "joj/resources/gltf/gltf_primitive.h"

joj::GLFTPrimitive::GLFTPrimitive()
    : position_acessor(-1)
    , normal_acessor(-1)
    , indices_acessor(-1)
    , material_index(-1)
    , mode(PrimitiveMode::UNKNOWN)
    , tangent_acessor(-1)
    , texcoord_acessor(-1)
    , color_acessor(-1)
    , joint_acessor(-1)
    , weight_acessor(-1)
{
}

joj::GLFTPrimitive::~GLFTPrimitive()
{
}