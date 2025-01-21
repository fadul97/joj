#ifndef _JOJ_SPRITE_H
#define _JOJ_SPRITE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "texture2d_data.h"

namespace joj
{
    struct SpriteData;

    class JAPI Sprite
    {
    public:
        Sprite();
        virtual ~Sprite();

        virtual void set_position(f32 x, f32 y) = 0;
        virtual void set_size(f32 width, f32 height) = 0;
        virtual void set_rotation(f32 angle) = 0;
        virtual void set_color(f32 r, f32 g, f32 b, f32 a = 1.0f) = 0;
        virtual void set_texture(const TextureData2D& texture) = 0;
        virtual void set_sprite_data(const SpriteData& data) = 0;

        virtual SpriteData& get_sprite_data() = 0;
    };
}

#endif // _JOJ_SPRITE_H