#ifndef _JOJ_FONT_H
#define _JOJ_FONT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "texture2d_data.h"

#include <string>

namespace joj
{
    struct FontData;

    class JAPI Font
    {
    public:
        Font();
        virtual ~Font();

        virtual void set_data(const FontData& data) = 0;
        virtual FontData& get_data() = 0;

        virtual void draw_text(const std::string& text, f32 x, f32 y) = 0;
    };
}

#endif // _JOJ_FONT_H