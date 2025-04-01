#ifndef _JOJ_ANIMATION_CHANNEL_TYPE_H
#define _JOJ_ANIMATION_CHANNEL_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class AnimationChannelType
    {
        TRANSLATION,
        ROTATION,
        SCALE,
        WEIGHTS,
        UNKNOWN
    };

    JOJ_API const char* animation_channel_type_to_string(const AnimationChannelType type);
}

#endif // _JOJ_ANIMATION_CHANNEL_TYPE_H