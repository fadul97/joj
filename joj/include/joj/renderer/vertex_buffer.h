#ifndef _JOJ_VERTEX_BUFFER_H
#define _JOJ_VERTEX_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "renderer.h"

namespace joj
{
    struct VBData;

    enum class BufferUsage
    {
        Default,
        Immutable,
        Dynamic,
        Staging,
    };

    enum class CPUAccessType
    {
        Write,
        Read
    };

    class JAPI VertexBuffer
    {
    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        virtual void setup(BufferUsage usage, CPUAccessType cpu_access_flags,
            u32 byte_width, const void* data) = 0;

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void bind(CommandList& cmd_list, u32 start_slot,
            u32 num_buffers, const u32* stride, const u32* offset) = 0;

        virtual VBData& get_data() = 0;

    protected:
        b8 m_filled;
    };
}

#endif // _JOJ_VERTEX_BUFFER_H