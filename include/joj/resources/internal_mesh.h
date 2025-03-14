#ifndef _JOJ_INTERNAL_MESH_H
#define _JOJ_INTERNAL_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "renderer/vertex.h"
#include <vector>

namespace joj
{
    struct JAPI InternalMesh
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