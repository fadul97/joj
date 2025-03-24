#ifndef _JOJ_BUFFER_USAGE_H
#define _JOJ_BUFFER_USAGE_H

namespace joj
{
    /**
     * @brief Type of usage for the Vertex and Index Buffers.
     * 
     * Default: The buffer will be used as a default buffer.
     * Immutatble: The buffer won't be updated.
     * Dynamic: The buffer will be updated frequently.
     * Stating: The buffer will be used as a staging buffer.
     */
    enum class BufferUsage
    {
        Default,
        Immutable,
        Dynamic,
        Staging,
    };
}

#endif // _JOJ_BUFFER_USAGE_H