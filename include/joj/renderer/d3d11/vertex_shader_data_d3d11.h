#ifndef _JOJ_D3D11_VERTEX_SHADER_DATA_H
#define _JOJ_D3D11_VERTEX_SHADER_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief VertexShader is a struct that holds the vertex shader data.
     * It is API dependent, so each API will have its own implementation.
     */
    struct JOJ_API VertexShader
    {
        /**
         * @brief The vertex shader data on the GPU.
         * It is used to bind the vertex shader to the pipeline.
         */
        ID3D11VertexShader* vertex_shader;

        /**
         * @brief Holds the bytecode of the vertex shader on the CPU.
         * It is used to create the vertex shader on the GPU.
         */
        ID3DBlob* vsblob;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_VERTEX_SHADER_DATA_H