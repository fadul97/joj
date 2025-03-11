#include "joj/renderer/d3d11/sprite_sheet_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

joj::D3D11SpriteSheet::D3D11SpriteSheet()
{
    m_data.texture.srv = nullptr;
    m_data.rows = 0;
    m_data.columns = 0;
    m_data.frame_width = 0;
    m_data.frame_height = 0;
}

joj::D3D11SpriteSheet::D3D11SpriteSheet(const i32 rows, const i32 columns,
    const i32 frame_width, const i32 frame_height)
{
    m_data.texture.srv = nullptr;
    m_data.rows = rows;
    m_data.columns = columns;
    m_data.frame_width = frame_width;
    m_data.frame_height = frame_height;
}

joj::D3D11SpriteSheet::~D3D11SpriteSheet()
{
}

void joj::D3D11SpriteSheet::set_data(const TextureData2D& texture_data)
{
    m_data.texture.srv = texture_data.srv;
}

joj::SpriteSheetData& joj::D3D11SpriteSheet::get_data()
{
    return m_data;
}

#endif // JOJ_PLATFORM_WINDOWS