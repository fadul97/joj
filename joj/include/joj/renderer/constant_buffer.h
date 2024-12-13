#ifndef _JOJ_CONSTANT_BUFFER_H
#define _JOJ_CONSTANT_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "renderer.h"

namespace joj
{
    struct CBData;

    class JAPI IConstantBuffer
    {
    public:
        IConstantBuffer();
        virtual ~IConstantBuffer();

        virtual void setup(const u32 byte_width, const void* data) = 0;

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void bind_to_vertex_shader(CommandList& cmd_list,
            u32 start_slot, u32 num_buffers) = 0;
        virtual void bind_to_pixel_shader(CommandList& cmd_list,
            u32 start_slot, u32 num_buffers) = 0;

        template <typename T>
        void update(CommandList& cmd_list, T& data);

        virtual CBData& get_data() = 0;

    protected:
        virtual void update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data) = 0;
    };

    template <typename T>
    inline void IConstantBuffer::update(CommandList& cmd_list, T& data)
    {
        // static_assert(sizeof(T) <= max_buffer_size, "Data size exceeds constant buffer limits.");
        update_internal(cmd_list, &data, sizeof(T));
    }
}

#endif // _JOJ_CONSTANT_BUFFER_H