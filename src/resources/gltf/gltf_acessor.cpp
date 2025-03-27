#include "joj/resources/gltf/gltf_acessor.h"

joj::GLTFAccessor::GLTFAccessor()
    : data_type(DataType::UNKNOWN), component_type(ComponentType::UNKNOWN),
    count(-1), buffer_view(-1), byte_offset(-1)
{
}

joj::GLTFAccessor::~GLTFAccessor()
{
}