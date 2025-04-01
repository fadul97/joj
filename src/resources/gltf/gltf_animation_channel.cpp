#include "joj/resources/gltf/gltf_animation_channel.h"

joj::GLTFAnimationChannel::GLTFAnimationChannel()
    : sampler(-1)
    , target_node(-1)
    , type(AnimationChannelType::UNKNOWN)
{
}

joj::GLTFAnimationChannel::~GLTFAnimationChannel()
{
}