#ifndef _JOJ_HELLO_TRIANGLE_H
#define _JOJ_HELLO_TRIANGLE_H

#include "joj/engine.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"
#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/texture2d_d3d11.h"
#include "joj/systems/camera/free_camera.h"
#include "joj/resources/d3d11/mesh_d3d11_old.h"
#include "joj/renderer/d3d11/sampler_state_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"
#include "joj/resources/d3d11/basic_model_d3d11.h"
#include "joj/renderer/d3d11/texture_manager_d3d11.h"
#include "joj/systems/light/light.h"
#include "joj/resources/d3d11/basic_skinned_model_d3d11.h"

struct cbPerSkinned
{
    joj::JFloat4x4 gBoneTransforms[96];
};

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

class HelloTriangle
{
public:
    HelloTriangle();
    ~HelloTriangle();

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

    f32 get_frametime();

    joj::Win32Window window;
    joj::Win32Input input;
    joj::Win32Timer timer;
    joj::D3D11Renderer renderer;
    b8 loop = true;
    f32 frametime = 0.0f;

    joj::RasterizerState m_raster_state = joj::RasterizerState::Solid;
    joj::FreeCamera m_cam;
    joj::D3D11SamplerState m_sampler_state;
    joj::D3D11InputLayout m_static_layout;
    joj::D3D11Shader m_static_shader;

    joj::D3D11ConstantBuffer cbObject;
    joj::D3D11ConstantBuffer cbFrame;

    joj::D3D11BasicModel m_cube_test;
    joj::D3D11TextureManager m_tex_mgr;
    joj::D3D11BasicModel m_rock_model;
    u32 ind = 0;
    void draw_one_object(u32 model_index);
    void draw_objects();
    joj::DirectionalLight mDirLights[3];
    joj::JFloat4x4 mShadowTransform = joj::float4x4_identity();
    std::vector<joj::BasicModelInstance> mModelInstances;
};

#endif // _JOJ_HELLO_TRIANGLE_H