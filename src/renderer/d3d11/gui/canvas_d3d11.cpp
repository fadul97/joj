#include "renderer/d3d11/gui/canvas_d3d11.h"

#if JPLATFORM_WINDOWS

joj::D3D11Canvas::D3D11Canvas()
{
}

joj::D3D11Canvas::D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height)
    : D3D11Widget(x, y, width, height)
{
}

joj::D3D11Canvas::D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color)
    : D3D11Widget(x, y, width, height, color)
{
}

joj::D3D11Canvas::~D3D11Canvas()
{
}

#endif // JPLATFORM_WINDOWS