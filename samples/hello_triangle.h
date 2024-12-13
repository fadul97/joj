#ifndef _JOJ_HELLO_TRIANGLE_H
#define _JOJ_HELLO_TRIANGLE_H

#include "joj/engine.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"

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
};

#endif // _JOJ_HELLO_TRIANGLE_H