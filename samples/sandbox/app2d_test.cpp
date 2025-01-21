#include "app2d_test.h"

#include "logger.h"
#include <sstream>
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

void App2DTest::setup_camera()
{
}

void App2DTest::build_shaders_and_input_layout()
{
    m_sprite_shader.compile_vertex_shader_from_file(
        "shaders/Sprite.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_sprite_shader.create_vertex_shader(m_renderer->get_device()));

    m_sprite_shader.compile_pixel_shader_from_file(
        "shaders/Sprite.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_sprite_shader.create_pixel_shader(m_renderer->get_device()));

    m_sprite_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_sprite_shader.bind_pixel_shader(m_renderer->get_cmd_list());

    joj::InputDesc layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 28, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : layout)
    {
        m_sprite_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_sprite_layout.create(m_renderer->get_device(), m_sprite_shader.get_vertex_shader()));
    m_sprite_layout.bind(m_renderer->get_cmd_list());
}

void App2DTest::load_sprites()
{
    JOJ_LOG_IF_FAIL(m_tex_manager.create(m_renderer->get_device(), m_renderer->get_cmd_list(),
        L"textures/shipanimated.dds", joj::ImageType::DDS));

    data = new joj::SpriteData();
    data->position = { 0.0f, 0.0f };
    data->size = { 1.0f, 1.0f };
    data->rotation = 0.0f;
    data->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    joj::TextureData2D& tex = m_tex_manager.get_texture(L"textures/shipanimated.dds");
    data->texture.srv = tex.srv;
    m_sprite.set_sprite_data(*data);

    joj::SpriteSheetData ssData;
    ssData.texture = tex;
    ssData.rows = 1;
    ssData.columns = 4;
    ssData.frame_width = 64;
    ssData.frame_height = 64;
    ssData.texture_width = 256;
    ssData.texture_height = 64;
    m_sprite.set_sprite_sheet_data(ssData);

    joj::SpriteAnimationData runAnim;
    runAnim.name = "Run";
    runAnim.frames = { 0, 1, 2, 3 };  // Quadro 0, 1, 2, 3 da SpriteSheet.
    runAnim.frameDuration = 0.1f;  // Cada quadro fica 0.1 segundos.

    m_sprite.add_animation(runAnim);

    using namespace joj;
    joj::Vertex::PosColorUVRect quad_vertices[] =
    {
        { JFloat3{ -0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f }, JFloat4{ 0.0f, 0.0f, 0.0f, 1.0f } }, // Top Left
        { JFloat3{  0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 0.0f, 1.0f, 1.0f }, JFloat4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // Top Right
        { JFloat3{  0.5f, -0.5f, 0.0f }, JFloat4{ 1.0f, 1.0f, 1.0f, 1.0f }, JFloat4{ 1.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Right
        { JFloat3{ -0.5f, -0.5f, 0.0f }, JFloat4{ 1.0f, 0.0f, 0.0f, 1.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Left
    };

    m_vb.setup(BufferUsage::Immutable, CPUAccessType::None,
        sizeof(Vertex::PosColorUVRect) * 4, quad_vertices);
    JOJ_LOG_IF_FAIL(m_vb.create(m_renderer->get_device()));

    // Índices para formar dois triângulos
    u32 quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_ib.setup(sizeof(u32) * 6, quad_indices);
    JOJ_LOG_IF_FAIL(m_ib.create(m_renderer->get_device()));
}

void App2DTest::build_cbs()
{
    cbObject.setup(joj::calculate_cb_byte_size(sizeof(CB2D)), nullptr);
    JOJ_LOG_IF_FAIL(cbObject.create(m_renderer->get_device()));
    cbObject.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    cbObject.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);
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
    setup_camera();
    build_shaders_and_input_layout();
    load_sprites();
    build_cbs();
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

b8 printed = false;
void App2DTest::draw_objects()
{
    joj::SpriteData& sprite = m_sprite.get_sprite_data();
    if (!printed)
    {
        JDEBUG("Sprite Position: %f, %f", sprite.position.x, sprite.position.y);
        JDEBUG("Sprite Size: %f, %f", sprite.size.x, sprite.size.y);
        JDEBUG("Sprite Rotation: %f", sprite.rotation);
    }
    
    joj::JMatrix4x4 scale = DirectX::XMMatrixScaling(sprite.size.x, sprite.size.y, 1.0f);
    joj::JMatrix4x4 rotation = DirectX::XMMatrixRotationZ(sprite.rotation);
    joj::JMatrix4x4 translation = DirectX::XMMatrixTranslation(sprite.position.x, sprite.position.y, 0.0f);
    joj::JMatrix4x4 world = scale * rotation * translation;

    // world = joj::matrix4x4_identity();
    // world = world;
    
    if (!printed)
    {
        JDEBUG("World Matrix:");
        joj::JFloat4x4 m;
        XMStoreFloat4x4(&m, world);
        joj::float4x4_print(m);
        printed = true;
    }

    CB2D cbData;
    XMStoreFloat4x4(&cbData.world, XMMatrixTranspose(world));
    cbData.color = sprite.color;
    cbData.uv_rect = m_sprite.get_sprite_data().uv_rect;
    cbObject.update(m_renderer->get_cmd_list(), cbData);

    m_sprite.draw();

    u32 stride = sizeof(joj::Vertex::PosColorUVRect);
    u32 offset = 0;

    m_vb.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_ib.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

    m_renderer->get_cmd_list().device_context->PSSetShaderResources(0, 1, &sprite.texture.srv);

    m_renderer->get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);
}

void App2DTest::shutdown()
{
    delete data;
}