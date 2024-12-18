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

    b8 load_model(char* filename);
    void release_model();

    joj::Win32Window window;
    joj::Win32Input input;
    joj::Win32Timer timer;
    joj::D3D11Renderer renderer;
    b8 loop = true;
    f32 frametime = 0.0f;

    joj::D3D11VertexBuffer m_vb;
    joj::D3D11IndexBuffer m_ib;
    joj::D3D11ConstantBuffer m_mat_cb;
    joj::D3D11ConstantBuffer m_light_cb;
    joj::D3D11Shader m_shader;
    joj::D3D11Texture2D m_tex;
    joj::FreeCamera m_cam;
};

#endif // _JOJ_HELLO_TRIANGLE_H