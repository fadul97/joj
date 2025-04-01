#ifndef _JOJ_GLTFSAMPLER_H
#define _JOJ_GLTFSAMPLER_H

#include "joj/core/defines.h"

#include "joj/resources/interpolation_type.h"
#include "joj/resources/animation_channel_type.h"

namespace joj
{
    struct JOJ_API GLTFSampler
    {
        GLTFSampler();
        ~GLTFSampler();

        InterpolationType interpolation;
        i32 input;
        i32 output;
        i32 target_node_index;
        AnimationChannelType type;
    };
}

#endif // _JOJ_GLTFSAMPLER_H