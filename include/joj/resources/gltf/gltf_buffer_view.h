#ifndef _JOJ_GLTFBUFFERVIEW_H
#define _JOJ_GLTFBUFFERVIEW_H

#include "joj/core/defines.h"

#include "joj/resources/buffer_view_target.h"

namespace joj
{
    struct JOJ_API GLTFBufferView
    {
        GLTFBufferView();
        ~GLTFBufferView();

        /** @brief The index of the buffer that contains this view. */
        i32 buffer;

        /** @brief The offset into the buffer in bytes. */
        i32 byte_offset;

        /** @brief The length of the bufferView in bytes. */
        i32 byte_length;
        
        /**
         * @brief The stride, in bytes, between vertex attributes or other interleaved data. 
         * For example: 3 * sizeof(float) for a buffer containing the data for 3 floats.
        */
        i32 byte_stride;

        /** @brief The target that the GPU buffer should be bound to. */
        BufferViewTarget target;
    };
}

#endif // _JOJ_GLTFBUFFERVIEW_H