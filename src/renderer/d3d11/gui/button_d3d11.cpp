#include "renderer/d3d11/gui/button_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

joj::D3D11Button::D3D11Button()
{
}

joj::D3D11Button::D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height)
    : D3D11Widget(x, y, width, height)
{
}

joj::D3D11Button::D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color)
    : D3D11Widget(x, y, width, height, color)
{
}

joj::D3D11Button::~D3D11Button()
{
}

#endif // JOJ_PLATFORM_WINDOWS