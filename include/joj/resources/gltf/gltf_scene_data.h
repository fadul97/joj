#ifndef _JOJ_GLTF_SCENE_DATA_H
#define _JOJ_GLTF_SCENE_DATA_H

#include "joj/core/defines.h"

#include <string>
#include <vector>

namespace joj
{
    struct JOJ_API GLTFSceneData
    {
        std::string name;
        std::vector<i32> root_nodes;
    };
}

#endif // _JOJ_GLTF_SCENE_DATA_H