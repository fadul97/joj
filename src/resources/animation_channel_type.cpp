#include "joj/resources/animation_channel_type.h"

const char* joj::animation_channel_type_to_string(const AnimationChannelType type)
{
    switch (type)
    {
        case AnimationChannelType::TRANSLATION: return "TRANSLATION";
        case AnimationChannelType::ROTATION:    return "ROTATION";
        case AnimationChannelType::SCALE:       return "SCALE";
        case AnimationChannelType::WEIGHTS:     return "WEIGHTS";
        default:                                return "UNKNOWN";
    }
}