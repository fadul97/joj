#ifndef _JOJ_FONT_H
#define _JOJ_FONT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "renderer.h"

namespace joj
{
    struct FontData;

    class JAPI Font
    {
    public:
        Font();
        Font(const char* filename);
        virtual ~Font();

        virtual void set_font(GraphicsDevice& device, CommandList& cmd_list, const char* filename) = 0;

        virtual void set_data(const FontData& data) = 0;
        virtual FontData& get_data() = 0;

        virtual void read_spacing(const char* filename) = 0;
        virtual void draw_text(const std::string& text, f32 x, f32 y) = 0;

    public:
        i16 m_spacing[256];
        b8 m_proportional;
        i32 m_char_width;
        u32 m_tex_width;
        u32 m_tex_height;
    };
}

#endif // _JOJ_FONT_H