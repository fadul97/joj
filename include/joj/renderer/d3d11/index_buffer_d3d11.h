#ifndef _JOJ_D3D11_INDEX_BUFFER_H
#define _JOJ_D3D11_INDEX_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/index_buffer.h"
#include <d3d11.h>

namespace joj
{
    struct IBData
    {
        D3D11_BUFFER_DESC ibd;
        ID3D11Buffer* index_buffer;
        D3D11_SUBRESOURCE_DATA init_data;
    };

    class JAPI D3D11IndexBuffer : public IndexBuffer
    {
    public:
        D3D11IndexBuffer();
        ~D3D11IndexBuffer();

        void setup(u32 byte_width, const void* data) override;

        ErrorCode create(const GraphicsDevice& device) override;

        void bind(CommandList& cmd_list, DataFormat format,
            const u32 offset) override;

        IBData& get_data() override;

    private:
        IBData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_INDEX_BUFFER_H