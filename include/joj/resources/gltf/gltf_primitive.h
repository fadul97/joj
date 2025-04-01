#ifndef _JOJ_GLTF_PRIMITIVE_H
#define _JOJ_GLTF_PRIMITIVE_H

#include "joj/core/defines.h"

#include "joj/resources/primitive_mode.h"

namespace joj
{
    struct JOJ_API GLFTPrimitive
    {
        GLFTPrimitive();
        ~GLFTPrimitive();

        i32 position_acessor;
        i32 normal_acessor;
        i32 indices_acessor;
        i32 material_index;
        PrimitiveMode mode;
    };
}

#endif // _JOJ_GLTF_PRIMITIVE_H