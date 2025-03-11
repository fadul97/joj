#include "joj/renderer/d3d11/font_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/logger.h"
#include "joj/renderer/d3d11/WICTextureLoader.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include <fstream>

joj::D3D11Font::D3D11Font()
{
    m_data.texture.srv = nullptr;
    m_data.char_width = 0;
    m_data.char_height = 0;
    m_data.texture_width = 0;
    m_data.texture_height = 0;
}

joj::D3D11Font::D3D11Font(const char* filename)
{
    (void)filename;

    m_data.texture.srv = nullptr;
    m_data.char_width = 0;
    m_data.char_height = 0;
    m_data.texture_width = 0;
    m_data.texture_height = 0;
}

joj::D3D11Font::~D3D11Font()
{
}

void joj::D3D11Font::set_font(GraphicsDevice& device, CommandList& cmd_list, const char* filename)
{
    if (m_data.texture.srv != nullptr)
    {
        m_data.texture.srv->Release();
        m_data.texture.srv = nullptr;
    }

    if (D3D11CreateTextureFromFile(device.device, cmd_list.device_context, filename,
        nullptr, &m_data.texture.srv, m_tex_width, m_tex_height) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
            "Failed to load Font file '%s'.", filename);
    }

    m_data.rows = 16;
    m_data.columns = 16;
    m_data.texture_width = m_tex_width;
    m_data.texture_height = m_tex_height;
    m_data.char_width = m_tex_width / m_data.columns;
    m_data.char_height = m_tex_height / m_data.rows;

    m_char_width = m_data.char_width;
}

void joj::D3D11Font::set_data(const FontData& data)
{
    m_data = data;
}

joj::FontData& joj::D3D11Font::get_data()
{
    return m_data;
}

void joj::D3D11Font::read_spacing(const char* filename)
{
    // Read 512 bytes (2 bytes per charactere)
    std::ifstream fin;
    fin.open(filename, std::ios_base::in | std::ios_base::binary);
    fin.read((char*)m_spacing, 512);
    fin.close();

    m_proportional = true;
}

void joj::D3D11Font::draw_text(const std::string& text, f32 x, f32 y)
{
    if (m_data.texture.srv == nullptr)
    {
        JOJ_WARN("Font texture is not set.");
        return;
    }
}

#endif // JOJ_PLATFORM_WINDOWS