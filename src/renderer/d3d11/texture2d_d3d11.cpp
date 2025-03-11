#include "renderer/d3d11/texture2d_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include <renderer/d3d11/WICTextureLoader.h>
#include "core/logger.h"
#include <locale>
#include <codecvt>

#include <windows.h>
#include <string>

std::string convert_wide_to_UTF8(const std::wstring& wide_str)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), (int)wide_str.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), (int)wide_str.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring convert_UTF8_to_wide(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wide_str(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wide_str[0], size_needed);
    return wide_str;
}


joj::D3D11Texture2D::D3D11Texture2D()
    : Texture2D()
{
    m_data.srv = nullptr;
}

joj::D3D11Texture2D::~D3D11Texture2D()
{
    // Release resources
    if (m_data.srv)
    {
        m_data.srv->Release();
        m_data.srv = nullptr;
    }
}

joj::ErrorCode joj::D3D11Texture2D::create(GraphicsDevice& device,
    CommandList& cmd_list, const std::wstring& filepath, ImageType type)
{
    switch (type)
    {
    case ImageType::PNG:
    {
        std::string str = convert_wide_to_UTF8(filepath);
        const char* filename = str.c_str();

        if (D3D11CreateTextureFromFile(
            device.device,
            cmd_list.device_context,
            filename,
            nullptr,
            &m_data.srv,
            m_width,
            m_height) != S_OK)
        {
            JOJ_ERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
                "Failed to create texture from PNG file '%s'.", filename);
            return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
        }
    }
    break;
    case ImageType::JPG:
    {
        std::string str = convert_wide_to_UTF8(filepath);
        const char* filename = str.c_str();

        if (D3D11CreateTextureFromFile(
            device.device,
            cmd_list.device_context,
            filename,
            nullptr,
            &m_data.srv,
            m_width,
            m_height) != S_OK)
        {
            JOJ_ERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
                "Failed to create texture from JPG file '%s'.", filename);
            return ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION;
        }
    }
    break;
    case ImageType::DDS:
        JOJ_TODO();
        break;
    default:
        break;
    }

    return ErrorCode::OK;
}

void joj::D3D11Texture2D::destroy()
{
    // Release resources
    if (m_data.srv)
    {
        m_data.srv->Release();
        m_data.srv = nullptr;
    }
}

void joj::D3D11Texture2D::bind(CommandList& cmd_list, u32 start_slot, u32 num_views)
{
    cmd_list.device_context->PSSetShaderResources(start_slot, num_views, &m_data.srv);
}

void joj::D3D11Texture2D::unbind(CommandList& cmd_list)
{
    ID3D11ShaderResourceView* null_srv[1] = { nullptr };
    cmd_list.device_context->PSSetShaderResources(0, 1, null_srv);
}

joj::TextureData2D& joj::D3D11Texture2D::get_data()
{
    return m_data;
}

#endif // JOJ_PLATFORM_WINDOWS