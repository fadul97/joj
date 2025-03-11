#ifndef _JOJ_SPRITE_SHEET_H
#define _JOJ_SPRITE_SHEET_H

#include "core/defines.h"

#include "texture2d_data.h"

namespace joj
{
    struct SpriteSheetData;

    class JAPI SpriteSheet
    {
    public:
        SpriteSheet();
        SpriteSheet(const i32 rows, const i32 columns,
            const i32 frame_width, const i32 frame_height);
        virtual ~SpriteSheet();

        virtual void set_data(const TextureData2D& texture_data) = 0;
        virtual SpriteSheetData& get_data() = 0;
    };
}

#endif // _JOJ_SPRITE_SHEET_H