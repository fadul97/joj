#ifndef _JOJ_APP_TEST_H
#define _JOJ_APP_TEST_H

// Includes --------------------------------------------------------------------
#include "joj/engine.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"

#include "joj/resources/d3d11/model_manager_d3d11.h"
#include "joj/resources/d3d11/basic_model_d3d11.h"
#include <vector>
#include "joj/systems/camera/free_camera.h"
#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"
#include "joj/systems/light/light.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"
#include "joj/renderer/d3d11/sampler_state_d3d11.h"

// Constants -------------------------------------------------------------------

struct cbPerObject
{
    joj::JFloat4x4 gWorld = joj::float4x4_identity();
    joj::JFloat4x4 gWorldInvTranspose = joj::float4x4_identity();
    joj::JFloat4x4 gWorldViewProj = joj::float4x4_identity();
    joj::JFloat4x4 gWorldViewProjTex = joj::float4x4_identity();
    joj::JFloat4x4 gTexTransform = joj::float4x4_identity();
    joj::JFloat4x4 gShadowTransform = joj::float4x4_identity();
    joj::Material gMaterial;
    u32 gUseTexure;
    u32 gAlphaClip;
    u32 gFogEnabled;
    u32 gReflectionEnabled;
};

struct cbPerFrame
{
    joj::DirectionalLight gDirLights[3];
    joj::JFloat3 gEyePosW;

    f32  gFogStart;
    f32  gFogRange;
    joj::JFloat4 gFogColor;
};

// Class -----------------------------------------------------------------------

class AppTest
{
public:
    AppTest();
    ~AppTest();

    void init();
    void update(const f32 dt);
    void draw();
    void shutdown();

    void init_platform();
    void setup_camera();
    void build_shaders_and_input_layout();
    void load_meshes_and_models();
    void build_cbs();
    void build_sampler_state();
    void draw_objects();

    f32 get_frametime();

    joj::Win32Window window;
    joj::Win32Input input;
    joj::Win32Timer timer;
    joj::D3D11Renderer renderer;
    b8 loop = true;
    f32 frametime = 0.0f;

    // ---------------------------------------------------
    joj::D3D11ModelManager m_model_manager;
    joj::D3D11BasicModel m_rock;
    std::vector<joj::BasicModelInstance> m_model_instances;
    joj::FreeCamera m_cam;
    joj::D3D11Shader m_static_shader;
    joj::D3D11InputLayout m_static_layout;
    joj::D3D11ConstantBuffer cbObject;
    joj::D3D11ConstantBuffer cbFrame;
    joj::D3D11SamplerState m_sampler_state;
    joj::DirectionalLight mDirLights[3];
};

#endif // _JOJ_APP_TEST_H