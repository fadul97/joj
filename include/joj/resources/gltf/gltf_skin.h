#ifndef _JOJ_GLTF_SKIN_H
#define _JOJ_GLTF_SKIN_H

#include "joj/core/defines.h"

#include <string>
#include <vector>

namespace joj
{
    struct JOJ_API GLTFSkin
    {
        GLTFSkin();
        ~GLTFSkin();

        std::string name;
        i32 inverse_bind_matrices_accessor_index;
        i32 skeleton_root_node_index;
        std::vector<i32> joint_indices;
    };
}

#endif // _JOJ_GLTF_SKIN_H