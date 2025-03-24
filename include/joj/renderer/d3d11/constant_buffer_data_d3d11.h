#ifndef _JOJ_D3D11_CONSTANT_BUFFER_DATA_H
#define _JOJ_D3D11_CONSTANT_BUFFER_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief ConstantBufferData is a struct that holds the Constant Buffer data.
     * It is API dependent, so each API will have its own implementation.
     */
    struct JOJ_API ConstantBufferData
    {
        D3D11_BUFFER_DESC cbd;
        ID3D11Buffer* constant_buffer;
        D3D11_SUBRESOURCE_DATA init_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_CONSTANT_BUFFER_DATA_H