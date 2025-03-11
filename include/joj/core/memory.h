#ifndef _JOJ_MEMORY_H
#define _JOJ_MEMORY_H

#include "defines.h"

namespace joj
{
    namespace Memory
    {
        /**
         * @brief Zero the given block of memory.
         *
         * @param block The block to be zeroed out.
         * @param size The size of data to zero out.
         * @return A pointer to the zeroed out block of memory.
         */
        JOJ_API void* zero(void* block, u64 size);
    }
}

#endif // _JOJ_MEMORY_H
