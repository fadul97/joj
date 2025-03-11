#ifndef _JOJ_D3D11_VERTEX_BUFFER_H
#define _JOJ_D3D11_VERTEX_BUFFER_H

#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/vertex_buffer.h"
#include <d3d11.h>

namespace joj
{
    struct VBData
    {
        D3D11_BUFFER_DESC vbd;
        ID3D11Buffer* vertex_buffer;
        D3D11_SUBRESOURCE_DATA init_data;
    };

    class JAPI D3D11VertexBuffer : public VertexBuffer
    {
    public:
        D3D11VertexBuffer();
        ~D3D11VertexBuffer();

        void setup(BufferUsage usage, CPUAccessType cpu_access_flags,
            u32 byte_width, const void* data) override;

        ErrorCode create(const GraphicsDevice& device) override;

        void bind(CommandList& cmd_list, u32 start_slot,
            u32 num_buffers, const u32* stride, const u32* offset) override;

        VBData& get_data() override;

        void cleanup() override;

    protected:
        void update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data) override;

    private:
        VBData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_VERTEX_BUFFER_H