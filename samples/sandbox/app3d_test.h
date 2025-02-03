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
#include "joj/systems/camera/free_camera.h"
#include "joj/systems/light/light.h"

// Constant Objects ------------------------------------------------------------

struct ConstantBuffer
{
    joj::JFloat4x4 wvp;
    joj::JFloat4x4 worldMatrix;
    joj::JFloat4x4 viewMatrix;
    joj::JFloat4x4 projectionMatrix;
};

struct LightCB
{
    joj::JVector4 diffuseColor;
    joj::JVector3 lightDirection;
    f32 padding;
};

struct MeshData
{
    std::vector<joj::Vertex::PosColorNormal> vertices;
    std::vector<u32> indices;
};

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

    void on_mouse_down(joj::Buttons button, i32 x, i32 y) override;
    void on_mouse_up(joj::Buttons button, i32 x, i32 y) override;
    void on_mouse_move(WPARAM button_state, i32 x, i32 y) override;

    void setup_camera();
    void build_shader();
    void build_input_layout();
    void build_buffers();

    void process_mouse_input(const f32 dt);

    void load_custom_format(const std::string& filename, MeshData& mesh);
    void load_custom_format_with_flat_shading(const std::string& filename, MeshData& mesh);
    void load_obj_format(const std::string& filename, MeshData& mesh);

    // ----------------------------------------------------
    joj::D3D11Shader m_shader;
    joj::D3D11InputLayout m_input_layout;
    joj::D3D11VertexBuffer m_vertex_buffer;
    joj::D3D11IndexBuffer m_index_buffer;
    joj::D3D11ConstantBuffer m_constant_buffer;

    joj::D3D11ModelManager m_model_mgr;
    joj::FreeCamera m_camera;
    joj::JFloat2 m_last_mouse_pos;

    joj::D3D11ConstantBuffer m_light_buffer;
};

#endif // _JOJ_3D_APP_TEST_H