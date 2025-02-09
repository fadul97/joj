#ifndef _JOJ_GUI_TEST_H
#define _JOJ_GUI_TEST_H

#include "joj/engine.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include "joj/renderer/d3d11/gui/gui_d3d11.h"

class GUITest : public joj::App
{
public:
    GUITest();
    ~GUITest();

    void init() override;
    void update(const f32 dt) override;
    void draw() override;
    void shutdown() override;

    // ---------------------------------------------------
    joj::D3D11GUI m_gui;
};

#endif // _JOJ_GUI_TEST_H