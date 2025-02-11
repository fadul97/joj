#ifndef _JOJ_INTERNAL_MESH_H
#define _JOJ_INTERNAL_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "math/jvector3.h"
#include "math/jvector4.h"
#include "math/jmath.h"

#include <vector>

namespace joj
{
    struct JAPI InternalMesh
    {
        std::vector<JVector3> positions;
        std::vector<JVector3> normals;
        std::vector<JFloat2> texCoords;
        std::vector<JVector4> colors;
        std::vector<uint32_t> indices;
    };
}

#endif // _JOJ_INTERNAL_MESH_H