#ifndef _JOJ_D3D11_TEXTURE_MANAGER_H
#define _JOJ_D3D11_TEXTURE_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "renderer/texture_manager.h"
#include "texture2d_data_d3d11.h"

namespace joj
{
    class JAPI D3D11TextureManager : public TextureManager
    {
    public:
        D3D11TextureManager();
        ~D3D11TextureManager();

        ErrorCode create(GraphicsDevice& device, CommandList& cmd_list,
            const std::wstring& filename, ImageType type) override;
        void destroy() override;

        TextureData2D& get_texture(const std::wstring& filename) override;

    private:
        std::map<std::wstring, TextureData2D> m_texture_SRV;
    };
}

#endif // _JOJ_D3D11_TEXTURE_MANAGER_H