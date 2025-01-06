#ifndef _JOJ_D3D11_GUI_H
#define _JOJ_D3D11_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/gui/gui.h"
#include <vector>
#include "renderer/d3d11/gui/canvas_d3d11.h"
#include "renderer/d3d11/gui/button_d3d11.h"

namespace joj
{
    class JAPI D3D11GUI : public IGUI
    {
    public:
        D3D11GUI();
        ~D3D11GUI();
        
        void init(GraphicsDevice& device) override;
        void update(const f32 dt, const i32 xmouse, const i32 ymouse) override;
        void draw(CommandList& cmd_list) override;
        void shutdown() override;

    private:
        std::vector<D3D11Widget*> m_widgets;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_GUI_H