#include "renderer/d3d11/texture_manager_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/DDSTextureLoader11.h"
#include <renderer/d3d11/WICTextureLoader.h>
#include "renderer/d3d11/renderer_d3d11.h"
#include "core/logger.h"
#include <codecvt>

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

    switch (type)
    {
    case joj::ImageType::PNG:
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(filename);
        const char* file = str.c_str();

        u32 width = 0;
        u32 height = 0;

        if (D3D11CreateTextureFromFile(
            device.device,
            cmd_list.device_context,
            file,
            nullptr,
            &srv,
            width,
            height) != S_OK)
        {
            JERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
                "Failed to create texture from PNG file '%s'.", filename);
            return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
        }
    }
        break;
    case joj::ImageType::JPG:
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(filename);
        const char* file = str.c_str();

        u32 width = 0;
        u32 height = 0;

        if (D3D11CreateTextureFromFile(
            device.device,
            cmd_list.device_context,
            file,
            nullptr,
            &srv,
            width,
            height) != S_OK)
        {
            JERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
                "Failed to create texture from JPG file '%s'.", filename);
            return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
        }
    }
        break;
    case joj::ImageType::DDS:
        if (DirectX::CreateDDSTextureFromFile(device.device, filename.c_str(),
            nullptr, &srv) != S_OK)
        {
            JERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
                "Failed to load DDS file '%ls'.", filename.c_str());
            return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
        }
        break;
    default:
        break;
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