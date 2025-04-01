#ifndef _JOJ_GLTF_ANIMATION_SAMPLER_H
#define _JOJ_GLTF_ANIMATION_SAMPLER_H

#include "joj/core/defines.h"

#include "joj/resources/interpolation_type.h"
#include "joj/resources/animation_channel_type.h"

namespace joj
{
    struct JOJ_API GLTFAnimationSampler
    {
        GLTFAnimationSampler();
        ~GLTFAnimationSampler();

        InterpolationType interpolation;
        i32 input;
        i32 output;
        i32 target_node_index;
        AnimationChannelType type;
    };
}

#endif // _JOJ_GLTF_ANIMATION_SAMPLER_H