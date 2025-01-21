#include "app2d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"

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
        L"textures/GravityGuy.dds", joj::ImageType::DDS));

    data = new joj::SpriteData();
    data->position = { 0.0f, 0.0f };
    data->size = { 1.0f, 1.0f };
    data->rotation = 0.0f;
    data->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    joj::TextureData2D& tex = m_tex_manager.get_texture(L"textures/GravityGuy.dds");
    data->texture.srv = tex.srv;
    m_sprite.set_sprite_data(*data);

    joj::SpriteSheetData ssData;
    ssData.texture = tex;
    ssData.rows = 2;
    ssData.columns = 5;
    ssData.frame_width = 32;
    ssData.frame_height = 48;
    ssData.texture_width = 160;
    ssData.texture_height = 96;
    m_sprite.set_sprite_sheet_data(ssData);

    joj::SpriteAnimationData runAnim;
    runAnim.name = "Run";
    runAnim.frames = { 0, 1, 2, 3, 4 };  // Quadro 0, 1, 2, 3, 4 da SpriteSheet.
    runAnim.frameDuration = 0.1f;  // Cada quadro fica 0.1 segundos.
    m_sprite.add_animation(runAnim);
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
    m_renderer->initialize_data2D();
    load_sprites();
    build_sampler_state();
    m_sprite.play_animation("Run");

    f32 x = m_sprite.get_sprite_data().uv_rect.x;
    f32 y = m_sprite.get_sprite_data().uv_rect.y;
    f32 z = m_sprite.get_sprite_data().uv_rect.z;
    f32 w = m_sprite.get_sprite_data().uv_rect.w;
    JDEBUG("UV Rect: (%f, %f, %f, %f)", x, y, z, w);
}

void App2DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    if (m_input->is_key_down(joj::KEY_W))
    {
        m_sprite.play_animation("Run");
        m_sprite.update(dt);
    }
}

void App2DTest::draw()
{
    m_renderer->clear();

    draw_objects();

    m_renderer->swap_buffers();
}

void App2DTest::draw_objects()
{
    m_sprite.draw();
    m_renderer->draw_sprite(m_sprite.get_sprite_data());
}

void App2DTest::shutdown()
{
    delete data;
}