#ifndef _JOJ_D3D11_INDEX_BUFFER_DATA_H
#define _JOJ_D3D11_INDEX_BUFFER_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief IndexBufferData is a struct that holds the Index Buffer data.
     * It is API dependent, so each API will have its own implementation.
     */
    struct JOJ_API IndexBufferData
    {
        D3D11_BUFFER_DESC ibd;
        ID3D11Buffer* index_buffer;
        D3D11_SUBRESOURCE_DATA init_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_INDEX_BUFFER_DATA_H