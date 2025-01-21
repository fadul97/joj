#ifndef _JOJ_D3D11_SPRITE_H
#define _JOJ_D3D11_SPRITE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/sprite.h"
#include "math/jmath.h"
#include "renderer/color.h"
#include "texture2d_d3d11.h"

namespace joj
{
    struct SpriteData
    {
        joj::JFloat2 position;
        joj::JFloat2 size;
        float rotation;
        joj::JFloat4 color;
        TextureData2D texture;
    };
    class JAPI D3D11Sprite : public Sprite
    {
    public:
        D3D11Sprite();
        ~D3D11Sprite();

        void set_position(f32 x, f32 y) override;
        void set_size(f32 width, f32 height) override;
        void set_rotation(f32 angle) override;
        void set_color(f32 r, f32 g, f32 b, f32 a = 1.0f) override;
        void set_texture(const TextureData2D& texture) override;
        void set_sprite_data(const SpriteData& data) override;

        SpriteData& get_sprite_data() override;

    private:
        SpriteData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_SPRITE_H