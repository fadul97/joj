#ifndef _JOJ_2D_APP_TEST_H
#define _JOJ_2D_APP_TEST_H

// Includes --------------------------------------------------------------------
#include "joj/application/app.h"

#include "joj/renderer/d3d11/texture_manager_d3d11.h"
#include "joj/renderer/d3d11/sampler_state_d3d11.h"
#include "joj/renderer/d3d11/sprite_d3d11.h"
#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/d3d11/input_layout_d3d11.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/constant_buffer_d3d11.h"
#include "joj/renderer/d3d11/font_d3d11.h"

// Constant Objects ------------------------------------------------------------

struct CBFont
{
    joj::JFloat4x4 world;
    joj::JFloat4 color;
    joj::JFloat4 uv_rect;
};

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

    void load_sprites();
    void build_sampler_state();
    void draw_objects();

    void setup_font();
    void draw_font();

    // ----------------------------------------------------
    joj::D3D11TextureManager m_tex_manager;;
    joj::D3D11SamplerState m_sampler_state;
    joj::D3D11Sprite m_sprite;
    joj::SpriteData* data;

    joj::D3D11Shader m_shader;
    joj::D3D11InputLayout m_input_layout;
    joj::D3D11VertexBuffer m_vertex_buffer;
    joj::D3D11IndexBuffer m_index_buffer;
    joj::D3D11ConstantBuffer m_constant_buffer;
    joj::D3D11Font m_font;
};

#endif // _JOJ_2D_APP_TEST_H