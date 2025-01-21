#ifndef _JOJ_2D_APP_TEST_H
#define _JOJ_2D_APP_TEST_H

// Includes --------------------------------------------------------------------
#include "joj/application/app.h"

#include "joj/resources/d3d11/model_manager_d3d11.h"
#include "joj/resources/d3d11/basic_model_d3d11.h"
#include <vector>
#include "joj/systems/camera/free_camera.h"
#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"
#include "joj/systems/light/light.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"
#include "joj/renderer/d3d11/sampler_state_d3d11.h"
#include "joj/renderer/d3d11/sprite_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"

// Constant Objects ------------------------------------------------------------


// Class -----------------------------------------------------------------------

class App2DTest : public joj::App
{
public:
    App2DTest();
    ~App2DTest();

    void init() override;
    void update(const f32 dt) override;
    void draw() override;
    void shutdown() override;

    void setup_camera();
    void load_sprites();
    void build_sampler_state();
    void draw_objects();

    // ----------------------------------------------------
    joj::D3D11TextureManager m_tex_manager;;
    joj::D3D11SamplerState m_sampler_state;
    joj::D3D11Sprite m_sprite;
    joj::SpriteData* data;
};

#endif // _JOJ_2D_APP_TEST_H