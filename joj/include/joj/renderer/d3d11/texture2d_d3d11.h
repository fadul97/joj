#ifndef _JOJ_D3D11_TEXTURE_2D_H
#define _JOJ_D3D11_TEXTURE_2D_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/texture2d.h"
#include <d3d11.h>

namespace joj
{
    struct Texture2DData
    {
        ID3D11ShaderResourceView* srv;
    };

    class JAPI D3D11Texture2D : public Texture2D
    {
    public:
        D3D11Texture2D();
        ~D3D11Texture2D();

        ErrorCode create(GraphicsDevice& device, CommandList& cmd_list,
            const std::wstring& filepath, ImageType type) override;
        void destroy() override;

        void bind(CommandList& cmd_list, u32 start_slot, u32 num_views) override;
        void unbind(CommandList& cmd_list) override;

        Texture2DData& get_data() override;
    protected:
        Texture2DData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_TEXTURE_2D_H