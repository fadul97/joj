#ifndef _JOJ_D3D11_TEXTURE_DATA_2D_H
#define _JOJ_D3D11_TEXTURE_DATA_2D_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    struct TextureData2D
    {
        ID3D11ShaderResourceView* srv;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_TEXTURE_DATA_2D_H