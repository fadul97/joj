#ifndef _JOJ_D3D11_FONT_H
#define _JOJ_D3D11_FONT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/font.h"
#include "texture2d_data_d3d11.h"

namespace joj
{
    struct FontData
    {
        TextureData2D texture;
        i32 rows;
        i32 columns;
        i32 char_width;
        i32 char_height;
        i32 texture_width;
        i32 texture_height;
    };

    class JAPI D3D11Font : public Font
    {
    public:
        D3D11Font();
        D3D11Font(const char* filename);
        ~D3D11Font();

        void set_font(GraphicsDevice& device, CommandList& cmd_list, const char* filename) override;

        void set_data(const FontData& data) override;
        FontData& get_data() override;

        void read_spacing(const char* filename) override;
        void draw_text(const std::string& text, f32 x, f32 y) override;

    private:
        FontData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_FONT_H