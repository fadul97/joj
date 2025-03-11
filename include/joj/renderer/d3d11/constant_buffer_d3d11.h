#ifndef _JOJ_D3D11_CONSTANT_BUFFER_H
#define _JOJ_D3D11_CONSTANT_BUFFER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/constant_buffer.h"
#include <d3d11.h>

namespace joj
{
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

    struct CBData
    {
        ID3D11Buffer* buffer;
        D3D11_SUBRESOURCE_DATA init_data;
        D3D11_BUFFER_DESC cbd;
    };

    class JOJ_API D3D11ConstantBuffer : public IConstantBuffer
    {
    public:
        D3D11ConstantBuffer();
        ~D3D11ConstantBuffer();

        void setup(const u32 byte_width, const void* data) override;

        ErrorCode create(const GraphicsDevice& device) override;

        void bind_to_vertex_shader(const CommandList& cmd_list,
            u32 start_slot, u32 num_buffers) override;
        void bind_to_pixel_shader(const CommandList& cmd_list,
            u32 start_slot, u32 num_buffers) override;

        CBData& get_data() override;

    protected:
        void update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data) override;

    private:
        CBData m_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_CONSTANT_BUFFER_H