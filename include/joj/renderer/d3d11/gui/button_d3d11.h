#ifndef _JOJ_D3D11_BUTTON_H
#define _JOJ_D3D11_BUTTON_H

#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/gui/widget_d3d11.h"

namespace joj
{
    class JAPI D3D11Button : public D3D11Widget
    {
    public:
        D3D11Button();
        D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height);
        D3D11Button(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        ~D3D11Button();
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_BUTTON_H