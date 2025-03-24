#ifndef _JOJ_D3D11_INPUT_LAYOUT_DATA_H
#define _JOJ_D3D11_INPUT_LAYOUT_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief InputLayoutData is a struct that holds the input layout data.
     * It is API dependent, so each API will have its own implementation.
     */
    struct JOJ_API InputLayoutData
    {
        /**
         * @brief The input layout data for the Vertex Shader.
         */
        ID3D11InputLayout* input_layout;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_INPUT_LAYOUT_DATA_H