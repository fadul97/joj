#include "joj/resources/gltf/gltf_animation_channel.h"

joj::GLTFAnimationChannel::GLTFAnimationChannel()
    : input(-1)
    , output(-1)
    , type(AnimationChannelType::UNKNOWN)
{
}

joj::GLTFAnimationChannel::~GLTFAnimationChannel()
{
}