#ifndef _JOJ_SPRITE_SHEET_H
#define _JOJ_SPRITE_SHEET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "texture2d_data.h"

namespace joj
{
    struct SpriteSheetData;

    class JAPI SpriteSheet
    {
    public:
        SpriteSheet();
        virtual ~SpriteSheet();

        virtual void set_data(const TextureData2D& texture_data) = 0;
        virtual SpriteSheetData& get_data() = 0;
    };
}

#endif // _JOJ_SPRITE_SHEET_H