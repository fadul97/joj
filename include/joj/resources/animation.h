#ifndef _JOJ_ANIMATION_H
#define _JOJ_ANIMATION_H

#include "joj/core/defines.h"

#include <string>
#include <vector>
#include "animation_channel.h"

namespace joj
{
    struct JOJ_API Animation
    {
        std::string name;
        std::vector<AnimationChannel> channels;
    };
}

#endif // _JOJ_ANIMATION_H