#include "joj/resources/gltf/gltf_sampler.h"

joj::GLTFSampler::GLTFSampler()
    : interpolation(InterpolationType::UNKNOWN)
    , input(-1)
    , output(-1)
    , target_node_index(-1)
    , type(AnimationChannelType::UNKNOWN)
{
}

joj::GLTFSampler::~GLTFSampler()
{
}