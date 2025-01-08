#ifndef _JOJ_D3D11_WIDGET_H
#define _JOJ_D3D11_WIDGET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/gui/widget.h"
#include "renderer/d3d11/shader_d3d11.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"
#include "renderer/d3d11/input_layout_d3d11.h"
#include "renderer/d3d11/constant_buffer_d3d11.h"
#include "math/jmath.h"

namespace joj
{
    struct CBHovered
    {
        JFloat4 hovered_color;
        i32 hovered;
    };

    class JAPI D3D11Widget : public Widget
    {
    public:
        D3D11Widget();
        D3D11Widget(const u16 x, const u16 y, const u16 width, const u16 height);
        D3D11Widget(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        ~D3D11Widget();

        ErrorCode create(GraphicsDevice& device) override;

        void draw(CommandList& cmd_list) override;

        b8 is_hovered(const i32 x, const i32 y) override;

        void update(const i32 xmouse, const i32 ymouse, const b8 clicked) override;
        void should_update() override;

        void set_background_color(const Color color) override;
        void set_hovered_color(const Color color) override;

        b8 on_left_edge(const i32 x, const i32 y) override;
        b8 on_right_edge(const i32 x, const i32 y) override;
        b8 on_top_edge(const i32 x, const i32 y) override;
        b8 on_bottom_edge(const i32 x, const i32 y) override;

    protected:
        D3D11Shader m_shader;
        D3D11VertexBuffer m_vb;
        D3D11IndexBuffer m_ib;
        D3D11ConstantBuffer m_cb;
        D3D11InputLayout m_input_layout;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_WIDGET_H