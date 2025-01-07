#include "renderer/d3d11/gui/button_d3d11.h"

#if JPLATFORM_WINDOWS

joj::D3D11Button::D3D11Button()
{
}

joj::D3D11Button::D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height,
    Widget* child)
    : D3D11Widget(x, y, width, height, child)
{
}

joj::D3D11Button::D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color, Widget* child)
    : D3D11Widget(x, y, width, height, color, child)
{
}

joj::D3D11Button::~D3D11Button()
{
}

#endif // JPLATFORM_WINDOWS