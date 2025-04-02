#ifndef _JOJ_3D_APP_TEST_H
#define _JOJ_3D_APP_TEST_H

// Includes --------------------------------------------------------------------
#include "joj/application/app.h"

#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"

#include "joj/systems/camera/free_camera.h"
#include "joj/systems/light/light.h"

#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"

#include "joj/resources/old_gltf_importer.h"
#include "joj/resources/gltf/gltf_importer.h"
#include "joj/resources/gltf/gltf_model.h"
#include "joj/resources/mesh.h"

// Constant Objects ------------------------------------------------------------

struct ConstantBuffer
{
    joj::JFloat4x4 wvp;
    joj::JFloat4x4 worldMatrix;
    joj::JFloat4x4 viewMatrix;
    joj::JFloat4x4 projectionMatrix;
};

struct LightBuffer
{
    joj::Vector4 diffuseColor;
    joj::Vector3 lightDirection;
    f32 padding;
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
    void build_buffers();

    void process_mouse_input(const f32 dt);

    // ----------------------------------------------------
    joj::D3D11VertexBuffer m_vb;
    joj::D3D11IndexBuffer m_ib;
    joj::D3D11ConstantBuffer m_cb;
    joj::D3D11ConstantBuffer m_lightcb;
    joj::D3D11Shader m_shader;

    joj::FreeCamera m_camera;
    joj::JFloat2 m_last_mouse_pos;

    joj::GLTFImporter m_model_importer;
    joj::GLTFModel* m_model;
};

#endif // _JOJ_3D_APP_TEST_H