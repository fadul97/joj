#include "joj/resources/animation_channel.h"

joj::AnimationChannel::AnimationChannel()
    : type(AnimationChannelType::UNKNOWN), target_node_index(-1)
{
}

joj::AnimationChannel::~AnimationChannel()
{
}