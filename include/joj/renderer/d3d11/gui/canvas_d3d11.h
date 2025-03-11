#ifndef _JOJ_D3D11_CANVAS_H
#define _JOJ_D3D11_CANVAS_H

#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/gui/widget_d3d11.h"

namespace joj
{
    class JAPI D3D11Canvas : public D3D11Widget
    {
    public:
        D3D11Canvas();
        D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height);
        D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        ~D3D11Canvas();
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_CANVAS_H