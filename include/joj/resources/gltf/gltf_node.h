#ifndef _JOJ_GLTF_NODE_H
#define _JOJ_GLTF_NODE_H

#include "joj/core/defines.h"

#include <vector>
#include "joj/core/math/matrix4x4.h"
#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"
#include <string>

namespace joj
{
    struct JOJ_API GLTFNode
    {
        GLTFNode();
        ~GLTFNode();

        i32 camera_index;
        std::vector<i32> children;
        i32 skin_index;
        Matrix4x4 matrix;        
        i32 mesh_index;
        Vector4 rotation; // Quaternion
        Vector3 scale;
        Vector3 translation;
        std::vector<i32> weights; // Morph target weights
        std::string name;        
    };

    JOJ_API b8 is_aggregator_node(const GLTFNode& node);

    JOJ_API Matrix4x4 compute_local_transform(const GLTFNode& node);
}

#endif // _JOJ_GLTF_NODE_H