#include "app2d_test.h"

#include "logger.h"
#include "joj/jmacros.h"
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

void App2DTest::setup_font()
{
    m_shader.compile_vertex_shader_from_file(
        "shaders/Font.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(m_renderer->get_device()));

    m_shader.compile_pixel_shader_from_file(
        "shaders/Font.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(m_renderer->get_device()));

    joj::InputDesc layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 28, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : layout)
    {
        m_input_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_input_layout.create(m_renderer->get_device(), m_shader.get_vertex_shader()));

    using namespace joj;
    joj::Vertex::PosColorUVRect quad_vertices[] =
    {
        { JFloat3{ -0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f }, JFloat4{ 0.0f, 0.0f, 0.0f, 1.0f } }, // Top Left
        { JFloat3{  0.5f,  0.5f, 0.0f }, JFloat4{ 0.0f, 0.0f, 1.0f, 1.0f }, JFloat4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // Top Right
        { JFloat3{  0.5f, -0.5f, 0.0f }, JFloat4{ 1.0f, 1.0f, 1.0f, 1.0f }, JFloat4{ 1.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Right
        { JFloat3{ -0.5f, -0.5f, 0.0f }, JFloat4{ 1.0f, 0.0f, 0.0f, 1.0f }, JFloat4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom Left
    };

    m_vertex_buffer.setup(BufferUsage::Immutable, CPUAccessType::None,
        sizeof(Vertex::PosColorUVRect) * 4, quad_vertices);
    JOJ_LOG_IF_FAIL(m_vertex_buffer.create(m_renderer->get_device()));

    // Índices para formar dois triângulos
    u32 quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_index_buffer.setup(sizeof(u32) * 6, quad_indices);
    JOJ_LOG_IF_FAIL(m_index_buffer.create(m_renderer->get_device()));

    m_constant_buffer.setup(joj::calculate_cb_byte_size(sizeof(CBFont)), nullptr);
    JOJ_LOG_IF_FAIL(m_constant_buffer.create(m_renderer->get_device()));
    m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_constant_buffer.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);

    m_font = D3D11Font();
    m_font.set_font(m_renderer->get_device(), m_renderer->get_cmd_list(), "textures/verdana12.png");
    m_font.read_spacing("textures/verdana12.dat");
}

void App2DTest::init()
{
    m_renderer->initialize_data2D();

    setup_font();

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

    // draw_objects();
    draw_font();

    m_renderer->swap_buffers();
}

void App2DTest::draw_objects()
{
    m_sprite.draw();
    m_renderer->draw_sprite(m_sprite.get_sprite_data());
}

void App2DTest::draw_font()
{
    m_shader.bind_vertex_shader(m_renderer->get_cmd_list());
    m_shader.bind_pixel_shader(m_renderer->get_cmd_list());
    m_input_layout.bind(m_renderer->get_cmd_list());
    m_constant_buffer.bind_to_vertex_shader(m_renderer->get_cmd_list(), 0, 1);
    m_constant_buffer.bind_to_pixel_shader(m_renderer->get_cmd_list(), 0, 1);

    auto sprite = m_font.get_data();

    f32 x = 0.0f; // posição de início do texto
    f32 y = 0.0f;

    // posição de início do texto
    float posX = x;
    float posY = y;

    f32 char_scale = 0.5f;

    std::string text = "Hello, World!";
    int textLength = int(text.size());

    // para cada caractere do texto
    for (int i = 0; i < textLength; ++i)
    {
        // caractere a ser exibido
        int frame = int(text[i]);

        // caracteres acentuados tem código deslocado
        if (frame < 0)
            frame += 256;

        // se foi carregado o espaçamento proporcional
        if (m_font.m_proportional)
            m_font.m_char_width = m_font.m_spacing[frame];
        
        // configura sprite
        joj::JMatrix4x4 scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
        joj::JMatrix4x4 rotation = DirectX::XMMatrixRotationZ(0);
        joj::JMatrix4x4 translation = DirectX::XMMatrixTranslation(posX / 50.0f, 0.0f, 0.0f);
        joj::JMatrix4x4 world = scale * rotation * translation;

        CBFont cb_data;
        XMStoreFloat4x4(&cb_data.world, XMMatrixTranspose(world));
        cb_data.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        // Calcular a linha e coluna no atlas
        int row = frame / sprite.rows;
        int col = frame % sprite.columns;

        // Calcular as coordenadas UV para o caractere
        float uv_min_x = float(col) / float(sprite.columns);
        float uv_min_y = float(row) / float(sprite.rows);
        float uv_max_x = float(col + 1) / float(sprite.columns);
        float uv_max_y = float(row + 1) / float(sprite.rows);

        // Definir o retângulo UV
        cb_data.uv_rect = { uv_min_x, uv_min_y, uv_max_x, uv_max_y };
        m_constant_buffer.update(m_renderer->get_cmd_list(), cb_data);

        u32 stride = sizeof(joj::Vertex::PosColorUVRect);
        u32 offset = 0;

        m_vertex_buffer.bind(m_renderer->get_cmd_list(), 0, 1, &stride, &offset);
        m_index_buffer.bind(m_renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);

        m_renderer->get_cmd_list().device_context->PSSetShaderResources(0, 1, &sprite.texture.srv);

        m_renderer->get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_renderer->get_cmd_list().device_context->DrawIndexed(6, 0, 0);

        // calcula posição do próximo caractere
        posX += m_font.get_data().char_width * char_scale;
        posY += m_font.get_data().char_width * char_scale;

        JDEBUG("Char: %c, posX: %f, posY: %f, char_width: %d", text[i], posX, posY, m_font.get_data().char_width);
    }
}

void App2DTest::shutdown()
{
    delete data;
}