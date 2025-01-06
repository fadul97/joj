#ifndef _JOJ_D3D11_GUI_H
#define _JOJ_D3D11_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/gui/gui.h"
#include "renderer/d3d11/gui/canvas_d3d11.h"

namespace joj
{
    class JAPI D3D11GUI : public IGUI
    {
    public:
        D3D11GUI();
        ~D3D11GUI();
        
        void init(GraphicsDevice& device) override;
        void update(const f32 dt) override;
        void draw(CommandList& cmd_list) override;
        void shutdown() override;

    private:
        D3D11Canvas m_canvas;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_GUI_H