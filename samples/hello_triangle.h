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
#include "joj/resources/d3d11/mesh_d3d11.h"
#include "joj/renderer/d3d11/sampler_state_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"

class HelloTriangle
{
public:
    HelloTriangle();
    ~HelloTriangle();

    void init();
    void update(const f32 dt);
    void draw();
    void shutdown();

    f32 get_frametime();

    joj::Win32Window window;
    joj::Win32Input input;
    joj::Win32Timer timer;
    joj::D3D11Renderer renderer;
    b8 loop = true;
    f32 frametime = 0.0f;

    joj::D3D11ConstantBuffer m_light_cb;
    joj::D3D11ConstantBuffer m_camera_cb;
    joj::FreeCamera m_cam;
    joj::D3D11MeshGeometryOld m_spaceship;
    joj::D3D11MeshGeometryOld m_spaceship2;
    joj::D3D11SamplerState m_sampler_state;
    joj::D3D11InputLayout m_input_layout;
};

#endif // _JOJ_HELLO_TRIANGLE_H