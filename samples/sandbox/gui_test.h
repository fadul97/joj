#ifndef _JOJ_GUI_TEST_H
#define _JOJ_GUI_TEST_H

// ----------------------------------------------------------------------------

#include "joj/application/app.h"

#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"

// ----------------------------------------------------------------------------

#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif

struct UIState
{
    i32 mousex;
    i32 mousey;
    i32 mousedown;

    i32 hotitem;
    i32 activeitem;

    i32 kbditem;
    i32 keyentered;
    i32 keymod;

    i32 lastwidget;
};

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
    void draw_rect(const i32 x, const i32 y, const i32 w, const i32 h, const f32 r, const f32 g, const f32 b, const f32 a);

    i32 regionhit(i32 x, i32 y, i32 w, i32 h);
    i32 button(i32 id, i32 x, i32 y, const f32 r, const f32 g, const f32 b, const f32 a);
    i32 slider(i32 id, i32 x, i32 y, i32 max, i32& value);
    void imgui_prepare();
    void imgui_finish();
    void render(const f32 dt);
    f32 m_dt = 0.0f;

    // ---------------------------------------------------

    joj::D3D11Shader m_shader;
    joj::D3D11VertexBuffer m_vb;
    joj::D3D11IndexBuffer m_ib;
    joj::D3D11ConstantBuffer m_cb;

    UIState uistate = { 0,0,0,0,0,0,0,0,0 };
};

#endif // _JOJ_GUI_TEST_H