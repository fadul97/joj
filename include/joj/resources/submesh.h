#ifndef _JOJ_SUBMESH_H
#define _JOJ_SUBMESH_H

#include "joj/core/defines.h"

#include "joj/core/math/matrix4x4.h"
#include <string>

namespace joj
{
    struct JOJ_API Submesh
    {
        Submesh();
        ~Submesh();

        std::string name;
        i32 vertex_start;
        i32 vertex_count;
        i32 index_start;
        i32 index_count;
    };
}

#endif // _JOJ_SUBMESH_H