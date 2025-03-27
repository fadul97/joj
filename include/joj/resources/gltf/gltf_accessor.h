#ifndef _JOJ_GLTF_ACCESSOR_H
#define _JOJ_GLTF_ACCESSOR_H

#include "joj/core/defines.h"

#include "joj/resources/data_type.h"
#include "joj/resources/component_type.h"

namespace joj
{
    struct JOJ_API GLTFAccessor
    {
        GLTFAccessor();
        ~GLTFAccessor();

        /** @brief The datatype of components in the attribute. */
        DataType data_type;

        /** @brief The datatype of components in the attribute. */
        ComponentType component_type;

        /** @brief The number of attributes referenced by this accessor. */
        i32 count;

        /** @brief The index of the bufferView. */
        i32 buffer_view;

        /** @brief The offset relative to the start of the bufferView in bytes. */
        i32 byte_offset;
    };
}

#endif // _JOJ_GLTF_ACCESSOR_H