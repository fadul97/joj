#ifndef _JOJ_D3D11_CANVAS_H
#define _JOJ_D3D11_CANVAS_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/gui/canvas.h"
#include "renderer/d3d11/shader_d3d11.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"
#include "renderer/d3d11/input_layout_d3d11.h"

namespace joj
{
    class JAPI D3D11Canvas : public Canvas
    {
    public:
        D3D11Canvas();
        D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height);
        D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color);
        ~D3D11Canvas();

        ErrorCode create(GraphicsDevice& device) override;

        void draw(CommandList& cmd_list) override;

    private:
        D3D11Shader m_shader;
        D3D11VertexBuffer m_vb;
        D3D11IndexBuffer m_ib;
        D3D11InputLayout m_input_layout;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_CANVAS_H