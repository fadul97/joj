#include "app2d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine.h"
#include <math.h>

b8 on_rect_collision(joj::Rect& rect1, joj::Rect& rect2)
{
    // Verificar sobreposição no eixo X
    b8 overlapX = rect1.get_right() >= rect2.get_left() &&
        rect1.get_left() <= rect2.get_right();

    // Verificar sobreposição no eixo Y
    b8 overlapY = rect1.get_bottom() >= rect2.get_top() &&
        rect1.get_top() <= rect2.get_bottom();

    // Existe colisão se há sobreposição nos dois eixos
    return overlapX && overlapY;
}


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
    data->position = { 400.0f, 300.0f };
    data->size = { 100.0f, 100.0f };
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
    runAnim.frames = { 1, 2, 3, 4 };  // Quadro 0, 1, 2, 3, 4 da SpriteSheet.
    runAnim.frameDuration = 0.1f;  // Cada quadro fica 0.1 segundos.
    m_sprite.add_animation(runAnim);

    runAnim.name = "IdleNormal";
    runAnim.frames = { 0 };  // Quadro 0 da SpriteSheet.
    runAnim.frameDuration = 0.1f;  // Cada quadro fica 0.1 segundos.
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

void App2DTest::setup_buffers()
{
    m_shader.compile_vertex_shader_from_file(
        "shaders/Physics.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(m_renderer->get_device()));

    m_shader.compile_pixel_shader_from_file(
        "shaders/Physics.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(m_renderer->get_device()));

    joj::InputDesc layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : layout)
    {
        m_input_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_input_layout.create(m_renderer->get_device(), m_shader.get_vertex_shader()));

    using namespace joj;
    joj::Vertex::PosColor quad_vertices[] =
    {
        { JFloat3{ -0.5f, -0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Left
        { JFloat3{  0.5f, -0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Right
        { JFloat3{ -0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Top Left
        { JFloat3{  0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Top Right
    };

    m_vertex_buffer.setup(BufferUsage::Immutable, CPUAccessType::None,
        sizeof(Vertex::PosColorUVRect) * 4, quad_vertices);
    JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

    m_constant_buffer.setup(joj::calculate_cb_byte_size(sizeof(CBPhysics)), nullptr);
    JOJ_LOG_IF_FAIL(m_constant_buffer.create(m_renderer->get_device()));
    m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_constant_buffer.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
}

void App2DTest::init()
{
    setup_buffers();

    load_sprites();
    build_sampler_state();
    m_sprite.play_animation("Run");

    f32 x = m_sprite.get_sprite_data().uv_rect.x;
    f32 y = m_sprite.get_sprite_data().uv_rect.y;
    f32 z = m_sprite.get_sprite_data().uv_rect.z;
    f32 w = m_sprite.get_sprite_data().uv_rect.w;
    JDEBUG("UV Rect: (%f, %f, %f, %f)", x, y, z, w);

    m_camera2D = joj::Camera2D(0, 800, 600, 0);
    m_camera2D.set_position({ 0.0f, 0.0f, 0.0f });

    m_scene.init(m_renderer->get_device(), m_camera2D);
    m_scene.add_sprite(&m_sprite);
}

void App2DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    if (m_input->is_key_down('K'))
    {
        m_sprite.play_animation("Run");
        m_sprite.update(dt);
        auto& sprite = m_sprite.get_sprite_data();
        sprite.position.x += 100.0f * dt;
        m_rect.set_position(sprite.position);
    }

    if (m_input->is_key_down('J'))
    {
        m_sprite.play_animation("Run");
        m_sprite.update(dt);
        auto& sprite = m_sprite.get_sprite_data();
        sprite.position.x -= 100.0f * dt;
        m_rect.set_position(sprite.position);
    }

    if (m_input->is_key_up('K') && m_input->is_key_up('J'))
    {
        m_sprite.play_animation("IdleNormal");
    }

    if (m_input->is_key_down('I'))
    {
        m_rect.set_size(200.0f, 200.0f);
    }

    if (m_input->is_key_down('R'))
    {
        m_rect2.set_size(200.0f, 10.0f);
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
}

void App2DTest::draw()
{
    m_renderer->clear();

    draw_sprites();
    draw_rect();

    m_renderer->swap_buffers();
}

void App2DTest::draw_sprites()
{
    m_scene.draw(*m_renderer);
}

void App2DTest::draw_rect()
{
    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_input_layout.bind(m_renderer->get_cmd_list());
    m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_constant_buffer.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);

    // Rect 1

    auto world = joj::matrix4x4_identity();
    world = DirectX::XMMatrixTranslation(m_rect.get_position2D().x, m_rect.get_position2D().y, 0.0f);
    auto scaleMatrix = DirectX::XMMatrixScaling(
        m_rect.get_size().x,
        m_rect.get_size().y,
        1.0f);
    world = scaleMatrix * world;
    auto view = DirectX::XMLoadFloat4x4(&m_camera2D.get_view());
    auto proj = DirectX::XMLoadFloat4x4(&m_camera2D.get_proj());
    auto wvp = world * view * proj;

    CBPhysics cb_data;
    XMStoreFloat4x4(&cb_data.wvp, XMMatrixTranspose(wvp));
    if (on_rect_collision(m_rect, m_rect2))
    {
        cb_data.color = { 1.0f, 0.0f, 0.0f, 1.0f };
    }
    else
    {
        cb_data.color = { 0.0f, 1.0f, 0.0f, 1.0f };
    }
    m_constant_buffer.update(m_renderer->get_cmd_list(), cb_data);

    u32 stride = sizeof(joj::Vertex::PosColor);
    u32 offset = 0;
    m_vertex_buffer.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);

    m_renderer->set_rasterizer_state(joj::RasterizerState::Wireframe);
    m_renderer->set_primitive_topology(joj::PrimitiveTopology::TRIANGLE_STRIP);
    m_renderer->get_cmd_list().device_context->Draw(4, 0);

    // Rect 2
    world = joj::matrix4x4_identity();
    world = DirectX::XMMatrixTranslation(m_rect2.get_position2D().x, m_rect2.get_position2D().y, 0.0f);
    scaleMatrix = DirectX::XMMatrixScaling(
        m_rect2.get_size().x,
        m_rect2.get_size().y,
        1.0f);
    world = scaleMatrix * world;
    view = DirectX::XMLoadFloat4x4(&m_camera2D.get_view());
    proj = DirectX::XMLoadFloat4x4(&m_camera2D.get_proj());
    wvp = world * view * proj;

    XMStoreFloat4x4(&cb_data.wvp, XMMatrixTranspose(wvp));
    m_constant_buffer.update(m_renderer->get_cmd_list(), cb_data);
    m_renderer->get_cmd_list().device_context->Draw(4, 0);
}

void App2DTest::draw_rect2()
{
}

void App2DTest::shutdown()
{
    delete data;
}