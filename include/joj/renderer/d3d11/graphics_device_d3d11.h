#ifndef _JOJ_D3D11_GRAPHICS_DEVICE_H
#define _JOJ_D3D11_GRAPHICS_DEVICE_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3d11.h>

namespace joj
{
    /**
     * @brief GraphicsDevice is a struct that holds the graphics device.
     * For DirectX 11, it will hold the ID3D11Device.
     */
    struct JOJ_API GraphicsDevice
    {
        ID3D11Device* device;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_GRAPHICS_DEVICE_H