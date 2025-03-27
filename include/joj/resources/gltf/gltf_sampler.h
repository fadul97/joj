#ifndef _JOJ_GLTFSAMPLER_H
#define _JOJ_GLTFSAMPLER_H

#include "joj/core/defines.h"

#include "joj/resources/interpolation_type.h"
#include <string>

namespace joj
{
    struct JOJ_API GLTFSampler
    {
        GLTFSampler();
        ~GLTFSampler();

        InterpolationType interpolation;
        i32 input;
        i32 output;
    };
}

#endif // _JOJ_GLTFSAMPLER_H