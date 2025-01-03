#include "renderer/d3d11/texture_manager_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/DDSTextureLoader11.h"
#include "renderer/d3d11/renderer_d3d11.h"
#include "logger.h"

joj::D3D11TextureManager::D3D11TextureManager()
{
}

joj::D3D11TextureManager::~D3D11TextureManager()
{
    for (auto it = m_texture_SRV.begin(); it != m_texture_SRV.end(); ++it)
    {
        it->second.srv->Release();
    }

    m_texture_SRV.clear();
}

joj::ErrorCode joj::D3D11TextureManager::create(GraphicsDevice& device,
    CommandList& cmd_list, const std::wstring& filename, ImageType type)
{
    ID3D11ShaderResourceView* srv = nullptr;

    /*
    if (m_texture_SRV.find(filename) != m_texture_SRV.end())
    {
        JWARN("Texture '%ls' was already loaded and will not be loaded again.",
            filename.c_str());
        return ErrorCode::OK;
    }
    */

    if (DirectX::CreateDDSTextureFromFile(device.device, filename.c_str(),
        nullptr, &srv) != S_OK)
    {
        JERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
            "Failed to load DDS file '%ls'.", filename.c_str());
        return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
    }

    m_texture_SRV[filename].srv = srv;

    return ErrorCode::OK;
}

void joj::D3D11TextureManager::destroy()
{
    for (auto it = m_texture_SRV.begin(); it != m_texture_SRV.end(); ++it)
    {
        it->second.srv->Release();
    }

    m_texture_SRV.clear();
}

joj::TextureData2D& joj::D3D11TextureManager::get_texture(const std::wstring& filename)
{
    if (m_texture_SRV.find(filename) != m_texture_SRV.end())
    {
        return m_texture_SRV[filename];
    }

    TextureData2D tex;
    tex.srv = nullptr;
    return tex;
}

#endif // JPLATFORM_WINDOWS