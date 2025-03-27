#ifndef _JOJ_ANIMATION_CHANNEL_H
#define _JOJ_ANIMATION_CHANNEL_H

#include "joj/core/defines.h"

#include "animation_channel_type.h"
#include <vector>
#include "keyframe.h"

namespace joj
{
    struct JOJ_API AnimationChannel
    {
        AnimationChannel();
        ~AnimationChannel();

        AnimationChannelType type;
        i32 target_node_index;
        std::vector<KeyFrame> keyframes;
    };
}

#endif // _JOJ_ANIMATION_CHANNEL_H