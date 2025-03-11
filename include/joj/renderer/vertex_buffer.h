#ifndef _JOJ_VERTEX_BUFFER_H
#define _JOJ_VERTEX_BUFFER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
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
        None,
        Write,
        Read
    };

    class JOJ_API VertexBuffer
    {
    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        virtual void setup(BufferUsage usage, CPUAccessType cpu_access_flags,
            u32 byte_width, const void* data) = 0;

        virtual ErrorCode create(const GraphicsDevice& device) = 0;

        virtual void bind(CommandList& cmd_list, u32 start_slot,
            u32 num_buffers, const u32* stride, const u32* offset) = 0;

        template <typename T>
        void update(CommandList& cmd_list, T& data);

        virtual VBData& get_data() = 0;

        b8 is_filled() const;
        virtual void cleanup() = 0;

    protected:
        b8 m_filled;

        virtual void update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data) = 0;
    };

    inline b8 VertexBuffer::is_filled() const
    { return m_filled; }

    template <typename T>
    inline void VertexBuffer::update(CommandList& cmd_list, T& data)
    {
        // static_assert(sizeof(T) <= max_buffer_size, "Data size exceeds constant buffer limits.");
        update_internal(cmd_list, &data, sizeof(T));
    }
}

#endif // _JOJ_VERTEX_BUFFER_H