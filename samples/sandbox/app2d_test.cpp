#include "app2d_test.h"

#include "joj/core/logger.h"
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"
#include <math.h>

App2DTest::App2DTest()
{
    data = nullptr;
}

App2DTest::~App2DTest()
{
}

void App2DTest::load_sprites()
{
    JOJ_LOG_IF_FAIL(m_tex_manager.create(m_renderer->get_device(), m_renderer->get_cmd_list(),
        L"textures/char_purple.png", joj::ImageType::PNG));

    data = new joj::SpriteData();
    data->position = { 400.0f, 300.0f };
    data->size = { 100.0f, 100.0f };
    data->rotation = 0.0f;
    data->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    joj::TextureData2D* tex = m_tex_manager.get_texture(L"textures/char_purple.png");
    data->texture.srv = tex->srv;
    m_sprite.set_sprite_data(*data);

    joj::SpriteSheetData ssData;
    ssData.texture = *tex;
    ssData.rows = 10;
    ssData.columns = 8;
    ssData.frame_width = 56;
    ssData.frame_height = 56;
    ssData.texture_width = 448;
    ssData.texture_height = 616;
    m_sprite.set_sprite_sheet_data(ssData);

    joj::SpriteAnimationData runAnim;
    runAnim.name = "Run";
    runAnim.frames = { 16, 17, 18, 19, 20, 21, 22, 23 };  // Quadro 0, 1, 2, 3 da SpriteSheet.
    runAnim.frame_duration = 0.1f;  // Cada quadro fica 0.1 segundos.
    m_sprite.add_animation(runAnim);

    runAnim.name = "Idle";
    runAnim.frames = { 0, 1, 2, 3, 4, 5 };  // Quadro 0 da SpriteSheet.
    runAnim.frame_duration = 0.1f;  // Cada quadro fica 0.1 segundos.
    m_sprite.add_animation(runAnim);

    runAnim.name = "Jump";
    runAnim.frames = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };  // Quadro 0 da SpriteSheet.
    runAnim.frame_duration = 0.1f;  // Cada quadro fica 0.1 segundos.
    m_sprite.add_animation(runAnim);

    m_rect = joj::Rect(data->size.x, data->size.y);
    m_rect2 = joj::Rect(50.0f, 50.0f);
    m_rect.set_position(data->position);
}

void App2DTest::build_sampler_state()
{
    // Describe a texture sampler.
    joj::SamplerDesc samplerDesc;
    samplerDesc.filter = joj::SamplerFilter::MIN_MAG_MIP_POINT;
    samplerDesc.addressU = joj::TextureAddressMode::Clamp;
    samplerDesc.addressV = joj::TextureAddressMode::Clamp;
    samplerDesc.addressW = joj::TextureAddressMode::Clamp;
    samplerDesc.mip_lod_bias = 0.0f;
    samplerDesc.max_anisotropy = 1;
    samplerDesc.func = joj::ComparisonFunc::Always;
    samplerDesc.border_color[0] = 0.0f;
    samplerDesc.border_color[0] = 0.0f;
    samplerDesc.border_color[0] = 0.0f;
    samplerDesc.border_color[0] = 0.0f;
    samplerDesc.min_LOD = joj::LODValue::Zero;
    samplerDesc.max_LOD = joj::LODValue::Float32_MAX;

    // Create Sampler State
    JOJ_LOG_IF_FAIL(m_sampler_state.create(m_renderer->get_device(), samplerDesc));
    m_sampler_state.bind(m_renderer->get_cmd_list(), joj::SamplerType::Anisotropic, 0, 1);
}

void App2DTest::init()
{
    load_sprites();
    build_sampler_state();
    m_sprite.play_animation("Run");

    m_camera2D = joj::Camera2D(0, 800, 600, 0);
    m_camera2D.set_position({ 0.0f, 0.0f, 0.0f });

    m_scene.init(m_renderer->get_device(), m_camera2D);
    m_scene.add_sprite(&m_sprite);

    m_scene.add_geometry(&m_rect);
    m_scene.add_geometry(&m_rect2);
}

static f32 rotation = 0.0f;
void App2DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    if (m_input->is_key_pressed('B'))
        m_debug_draw = !m_debug_draw;

    // m_sprite.play_animation("Idle");
    m_sprite.update(dt);

    if (m_input->is_key_down('K'))
    {
        m_sprite.play_animation("Run");
        auto& sprite = m_sprite.get_sprite_data();
        sprite.position.x += 100.0f * dt;
        m_rect.set_position(sprite.position);
    }

    if (m_input->is_key_down('J'))
    {
        m_sprite.play_animation("Run");
        auto& sprite = m_sprite.get_sprite_data();
        sprite.position.x -= 100.0f * dt;
        m_rect.set_position(sprite.position);
    }

    if (m_input->is_key_down(joj::KEY_SPACE))
    {
        m_sprite.play_animation("Jump");
    }

    if (m_input->is_key_up('K') && m_input->is_key_up('J') && m_input->is_key_up(joj::KEY_SPACE))
    {
        m_sprite.play_animation("Idle");
    }

    if (m_input->is_key_down('I'))
    {
        m_rect.set_size(200.0f, 200.0f);
    }

    if (m_input->is_key_down('D'))
    {
        m_rect.translate(0.5f, 0.0f);
    }
    if (m_input->is_key_down('A'))
    {
        m_rect.translate(-0.5f, 0.0f);
    }
    if (m_input->is_key_down('W'))
    {
        m_rect.translate(0.0f, -0.5f);
    }
    if (m_input->is_key_down('S'))
    {
        m_rect.translate(0.0f, +0.5f);
    }

    if (m_input->is_key_down(joj::KEY_RIGHT))
    {
        m_rect2.translate(0.5f, 0.0f);
    }
    if (m_input->is_key_down(joj::KEY_LEFT))
    {
        m_rect2.translate(-0.5f, 0.0f);
    }
    if (m_input->is_key_down(joj::KEY_UP))
    {
        m_rect2.translate(0.0f, -0.5f);
    }
    if (m_input->is_key_down(joj::KEY_DOWN))
    {
        m_rect2.translate(0.0f, +0.5f);
    }

    m_rect.check_collision(m_rect2);
    m_rect2.check_collision(m_rect);
}

void App2DTest::draw()
{
    m_renderer->clear();

    draw_sprites();

    if (m_debug_draw)
        m_scene.draw_collisions(*m_renderer);

    m_renderer->swap_buffers();
}

void App2DTest::draw_sprites()
{
    m_scene.draw(*m_renderer);
}

void App2DTest::shutdown()
{
    delete data;
}