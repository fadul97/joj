#ifndef _JOJ_INTERNAL_MESH_H
#define _JOJ_INTERNAL_MESH_H

#include "joj/core/defines.h"

#include "joj/renderer/vertex.h"
#include <vector>

namespace joj
{
    struct JOJ_API InternalMesh
    {
        std::vector<Vertex::PosColorNormal> vertices;
        std::vector<u32> indices;
        /*
        std::vector<JVector3> positions;
        std::vector<JVector3> normals;
        std::vector<JFloat2> texCoords;
        std::vector<JVector4> colors;
        */
    };
}

#endif // _JOJ_INTERNAL_MESH_H