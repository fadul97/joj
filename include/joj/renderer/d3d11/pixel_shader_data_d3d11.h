#ifndef _JOJ_D3D11_PIXEL_SHADER_DATA_H
#define _JOJ_D3D11_PIXEL_SHADER_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief PixelShader is a struct that holds the pixel shader data.
     * It is API dependent, so each API will have its own implementation.
     */
    struct JOJ_API PixelShader
    {
        /**
         * @brief The pixel shader data on the GPU.
         * It is used to bind the pixel shader to the pipeline.
         */
        ID3D11PixelShader* pixel_shader;

        /**
         * @brief Holds the bytecode of the pixel shader on the CPU.
         * It is used to create the pixel shader on the GPU.
         */
        ID3DBlob* psblob;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_PIXEL_SHADER_DATA_H