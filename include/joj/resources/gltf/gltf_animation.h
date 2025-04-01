#ifndef _JOJ_GLTF_ANIMATION_H
#define _JOJ_GLTF_ANIMATION_H

#include "joj/core/defines.h"

#include <string>
#include <vector>
#include "gltf_animation_channel.h"
#include "gltf_animation_sampler.h"

namespace joj
{
    struct JOJ_API GLTFAnimation
    {
        std::string name;
        std::vector<GLTFAnimationChannel> channels;
        std::vector<GLTFAnimationSampler> samplers;
    };
}

#endif // _JOJ_GLTF_ANIMATION_H