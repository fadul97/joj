#include "renderer/d3d11/sprite_d3d11.h"

#if JPLATFORM_WINDOWS

joj::D3D11Sprite::D3D11Sprite()
{
    m_data.position = { 0.0f, 0.0f };
    m_data.size = { 0.0f, 0.0f };
    m_data.rotation = 0.0f;
    m_data.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_data.texture.srv = nullptr;
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

joj::SpriteData& joj::D3D11Sprite::get_sprite_data()
{
    return m_data;
}

#endif // JPLATFORM_WINDOWS