#ifndef _JOJ_OBJ_LOADER_H
#define _JOJ_OBJ_LOADER_H

#include "joj/core/defines.h"

#include "internal_mesh.h"
#include <vector>

namespace joj
{
    class JOJ_API OBJLoader
    {
    public:
        OBJLoader();
        ~OBJLoader();

        InternalMesh* load(const char* filename);

    private:
        std::vector<InternalMesh*> m_meshes;
    };
}

#endif // _JOJ_OBJ_LOADER_H