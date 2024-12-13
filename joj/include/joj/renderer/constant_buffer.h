#ifndef _JOJ_CONSTANT_BUFFER_H
#define _JOJ_CONSTANT_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "renderer.h"

namespace joj
{
    struct CBData;

    inline u32 calculate_cb_byte_size(u32 byte_size)
    {
        // Constant buffers must be a multiple of the minimum hardware
        // allocation size (usually 256 bytes).  So round up to nearest
        // multiple of 256.  We do this by adding 255 and then masking off
        // the lower 2 bytes which store all bits < 256.
        // Example: Suppose byte_size = 300.
        // (300 + 255) & ~255
        // 555 & ~255
        // 0x022B & ~0x00ff
        // 0x022B & 0xff00
        // 0x0200
        // 512
        return (byte_size + 255) & ~255;
    }

    class JAPI IConstantBuffer
    {
    public:
        IConstantBuffer();
        virtual ~IConstantBuffer();

        virtual void setup(const u32 byte_width, const void* data) = 0;

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void bind(CommandList& cmd_list, u32 start_slot,
            u32 num_buffers, const u32* stride, const u32* offset) = 0;

        virtual void update(CommandList& cmd_list, void* data, const u32 sizeof_data) = 0;

        virtual CBData& get_data() = 0;
    };
}

#endif // _JOJ_CONSTANT_BUFFER_H