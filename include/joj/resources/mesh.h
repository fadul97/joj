#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#include "joj/core/defines.h"

#include <vector>
#include "joj/renderer/vertex.h"
#include "submesh.h"

namespace joj
{
    struct JOJ_API Mesh
    {
        std::vector<Vertex::ColorTanPosNormalTex> vertices;
        std::vector<u16> indices;
        std::vector<Submesh> submeshes;
    };
}

#endif // _JOJ_MESH_H