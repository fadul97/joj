#ifndef _JOJ_GLTF_MESH_H
#define _JOJ_GLTF_MESH_H

#include "joj/core/defines.h"

#include <string>
#include <vector>
#include "gltf_primitive.h"

namespace joj
{
    struct JOJ_API GLTFMesh
    {
        std::string name;
        std::vector<u32> weights;
        std::vector<GLTFPrimitive> primitives;
    };
}

#endif // _JOJ_GLTF_MESH_H