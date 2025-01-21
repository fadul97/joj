#ifndef _JOJ_D3D11_SPRITE_SHEET_H
#define _JOJ_D3D11_SPRITE_SHEET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/sprite_sheet.h"
#include "texture2d_data_d3d11.h"

namespace joj
{
    struct SpriteSheetData
    {
        TextureData2D texture; // A textura que contém os quadros.
        i32 rows;              // Número de linhas na SpriteSheet.
        i32 columns;           // Número de colunas na SpriteSheet.
        i32 frame_width;       // Largura de cada quadro.
        i32 frame_height;      // Altura de cada quadro.
    };

    class JAPI D3D11SpriteSheet : public SpriteSheet
    {
    public:
        D3D11SpriteSheet();
        ~D3D11SpriteSheet();

        void set_data(const TextureData2D& texture_data) override;
        SpriteSheetData& get_data() override;
    private:
        SpriteSheetData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_SPRITE_SHEET_H