#include "renderer/d3d11/sprite_sheet_d3d11.h"

#if JPLATFORM_WINDOWS

joj::D3D11SpriteSheet::D3D11SpriteSheet()
{
    m_data.texture.srv = nullptr;
    m_data.rows = 0;
    m_data.columns = 0;
    m_data.frame_width = 0;
    m_data.frame_height = 0;
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

#endif // JPLATFORM_WINDOWS