#include "joj/renderer/d3d11/sprite_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/logger.h"

joj::D3D11Sprite::D3D11Sprite()
{
    m_data.texture.srv = nullptr;
    m_data.position = { 0.0f, 0.0f };
    m_data.size = { 0.0f, 0.0f };
    m_data.rotation = 0.0f;
    m_data.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_data.uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };

    m_sprite_sheet_data.texture.srv = nullptr;
    m_sprite_sheet_data.rows = 0;
    m_sprite_sheet_data.columns = 0;
    m_sprite_sheet_data.frame_width = 0;
    m_sprite_sheet_data.frame_height = 0;
}

joj::D3D11Sprite::~D3D11Sprite()
{
}

void joj::D3D11Sprite::set_position(f32 x, f32 y)
{
    m_data.position = { x, y };
}

void joj::D3D11Sprite::set_size(f32 width, f32 height)
{
    m_data.size = { width, height };
}

void joj::D3D11Sprite::set_rotation(f32 angle)
{
    m_data.rotation = angle;
}

void joj::D3D11Sprite::set_color(f32 r, f32 g, f32 b, f32 a)
{
    m_data.color = { r, g, b, a };
}

void joj::D3D11Sprite::set_texture(const TextureData2D& texture)
{
    m_data.texture.srv = texture.srv;
}

void joj::D3D11Sprite::set_sprite_data(const SpriteData& data)
{
    m_data = data;
}

void joj::D3D11Sprite::set_sprite_sheet_data(const SpriteSheetData& data)
{
    m_sprite_sheet_data = data;
}

joj::SpriteData& joj::D3D11Sprite::get_sprite_data()
{
    return m_data;
}

void joj::D3D11Sprite::draw()
{
    i32 frame = m_current_animation->frames[m_current_frame_index];
    i32 x = (frame % m_sprite_sheet_data.columns) * m_sprite_sheet_data.frame_width;
    i32 y = (frame / m_sprite_sheet_data.columns) * m_sprite_sheet_data.frame_height;

    m_data.uv_rect =
    {
        x / static_cast<f32>(m_sprite_sheet_data.texture_width),
        y / static_cast<f32>(m_sprite_sheet_data.texture_height),
        (x + m_sprite_sheet_data.frame_width) / static_cast<float>(m_sprite_sheet_data.texture_width), // UV X final
        (y + m_sprite_sheet_data.frame_height) / static_cast<float>(m_sprite_sheet_data.texture_height) // UV Y final
    };
}

#endif // JOJ_PLATFORM_WINDOWS