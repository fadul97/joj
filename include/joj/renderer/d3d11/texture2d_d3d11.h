#ifndef _JOJ_D3D11_TEXTURE_2D_H
#define _JOJ_D3D11_TEXTURE_2D_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/texture2d.h"
#include "texture2d_data_d3d11.h"

namespace joj
{
    class JOJ_API D3D11Texture2D : public Texture2D
    {
    public:
        D3D11Texture2D();
        ~D3D11Texture2D();

        ErrorCode create(GraphicsDevice& device, CommandList& cmd_list,
            const std::wstring& filepath, ImageType type) override;
        void destroy() override;

        void bind(CommandList& cmd_list, u32 start_slot, u32 num_views) override;
        void unbind(CommandList& cmd_list) override;

        TextureData2D& get_data() override;
    protected:
        TextureData2D m_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_TEXTURE_2D_H