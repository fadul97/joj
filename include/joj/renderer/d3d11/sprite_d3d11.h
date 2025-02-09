#ifndef _JOJ_D3D11_SPRITE_H
#define _JOJ_D3D11_SPRITE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/sprite.h"
#include "math/jmath.h"
#include "renderer/color.h"
#include "texture2d_d3d11.h"
#include "sprite_sheet_d3d11.h"
#include "renderer/sprite_animation_data.h"

namespace joj
{
    struct SpriteData
    {
        TextureData2D texture;
        JFloat2 position;
        JFloat2 size;
        f32 rotation;
        JFloat4 color;
        JFloat4 uv_rect;
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
        void set_sprite_sheet_data(const SpriteSheetData& data) override;

        SpriteData& get_sprite_data() override;

        virtual void draw() override;

    private:
        SpriteData m_data;
        SpriteSheetData m_sprite_sheet_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_SPRITE_H