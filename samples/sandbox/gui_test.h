#ifndef _JOJ_GUI_TEST_H
#define _JOJ_GUI_TEST_H

// ----------------------------------------------------------------------------

#include "joj/application/app.h"

#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"

// ----------------------------------------------------------------------------

class GUITest : public joj::App
{
public:
    GUITest();
    ~GUITest();

    void init() override;
    void update(const f32 dt) override;
    void draw() override;
    void shutdown() override;

    void setup_quad_buffers_and_shaders();
    void draw_quad();
    void draw_rect(const i32 x, const i32 y, const i32 w, const i32 h, const i32 r, const i32 g, const i32 b, const i32 a);

    // ---------------------------------------------------

    joj::D3D11Shader m_shader;
    joj::D3D11InputLayout m_layout;
    joj::D3D11VertexBuffer m_vb;
    joj::D3D11IndexBuffer m_ib;
    joj::D3D11ConstantBuffer m_cb;
};

#endif // _JOJ_GUI_TEST_H