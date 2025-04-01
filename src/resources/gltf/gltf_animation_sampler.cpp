#include "joj/resources/gltf/gltf_animation_sampler.h"

joj::GLTFAnimationSampler::GLTFAnimationSampler()
    : interpolation(InterpolationType::UNKNOWN)
    , input(-1)
    , output(-1)
    , target_node_index(-1)
    , type(AnimationChannelType::UNKNOWN)
{
}

joj::GLTFAnimationSampler::~GLTFAnimationSampler()
{
}