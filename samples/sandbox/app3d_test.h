#ifndef _JOJ_3D_APP_TEST_H
#define _JOJ_3D_APP_TEST_H

// Includes --------------------------------------------------------------------
#include "joj/application/app.h"

#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"

#include "joj/resources/d3d11/model_manager_d3d11.h"

// Constant Objects ------------------------------------------------------------

// Class -----------------------------------------------------------------------

class App3DTest : public joj::App
{
public:
    App3DTest();
    ~App3DTest();

    void init() override;
    void update(const f32 dt) override;
    void draw() override;
    void shutdown() override;

    void build_shader();
    void build_input_layout();
    void build_buffers();

    // ----------------------------------------------------
    joj::D3D11Shader m_shader;
    joj::D3D11InputLayout m_input_layout;
    joj::D3D11VertexBuffer m_vertex_buffer;
    joj::D3D11IndexBuffer m_index_buffer;
    joj::D3D11ConstantBuffer m_constant_buffer;

    joj::D3D11ModelManager m_model_mgr;
};

#endif // _JOJ_3D_APP_TEST_H