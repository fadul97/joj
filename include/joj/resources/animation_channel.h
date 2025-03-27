#ifndef _JOJ_ANIMATION_CHANNEL_H
#define _JOJ_ANIMATION_CHANNEL_H

#include "joj/core/defines.h"

#include <vector>
#include "keyframe.h"
#include "animation_channel_type.h"

namespace joj
{
    struct JOJ_API AnimationChannel
    {
        AnimationChannel();
        ~AnimationChannel();

        std::vector<KeyFrame> keyframes;
        AnimationChannelType type;

        // TODO: Check if this is necessary
        i32 target_node_index;
    };
}

#endif // _JOJ_ANIMATION_CHANNEL_H