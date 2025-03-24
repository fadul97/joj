#ifndef _JOJ_D3D11_COMMAND_LIST_H
#define _JOJ_D3D11_COMMAND_LIST_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief CommandList is a struct that holds the command list.
     * For DirectX 11, it will hold the ID3D11DeviceContext.
     */
    struct JOJ_API CommandList
    {
        ID3D11DeviceContext* device_context;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_COMMAND_LIST_H