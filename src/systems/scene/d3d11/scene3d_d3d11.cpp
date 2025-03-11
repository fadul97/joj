#include "joj/systems/scene/d3d11/scene3d_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/jmacros.h"
#include "joj/renderer/d3d11/sprite_d3d11.h"
#include "joj/renderer/vertex.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include "joj/systems/physics/rect.h"
#include "joj/systems/physics/collisions.h"

joj::D3D11Scene3D::D3D11Scene3D()
{
    m_model_vb = D3D11VertexBuffer();
    m_model_ib = D3D11IndexBuffer();
    m_model_cb = D3D11ConstantBuffer();
    m_model_shader = D3D11Shader();
    m_model_layout = D3D11InputLayout();
}

joj::D3D11Scene3D::~D3D11Scene3D()
{
}

struct ConstantBuffer2D
{
    joj::JFloat4x4 world;
    joj::JFloat4 color;
    joj::JFloat4 uv_rect;
};

void joj::D3D11Scene3D::init(const GraphicsDevice& device, Camera& camera)
{
    m_camera = &camera;
    
    // ----------------------------------------------------
    // Model Setup
    // ----------------------------------------------------

    m_model_shader.compile_vertex_shader_from_file(
        "shaders/Sprite.hlsl",
        "VS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_model_shader.create_vertex_shader(device));

    m_model_shader.compile_pixel_shader_from_file(
        "shaders/SimpleModel.hlsl",
        "PS", joj::ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_model_shader.create_pixel_shader(device));

    joj::InputDesc model_layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,       0, 28, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : model_layout)
    {
        m_model_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_model_layout.create(device, m_model_shader.get_vertex_shader()));

    /*
    m_model_vb.setup(BufferUsage::Immutable, CPUAccessType::None,
        sizeof(Vertex::PosColorUVRect) * 4, sprite_quad_vertices);
    JOJ_LOG_IF_FAIL(m_model_vb.create(device));
    */

    // �ndices para formar dois tri�ngulos
    u32 quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_model_ib.setup(sizeof(u32) * 6, quad_indices);
    JOJ_LOG_IF_FAIL(m_model_ib.create(device));

    m_model_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer2D)), nullptr);
    JOJ_LOG_IF_FAIL(m_model_cb.create(device));
}

void joj::D3D11Scene3D::update(const f32 dt)
{
}

void joj::D3D11Scene3D::draw(IRenderer& renderer)
{
    m_model_shader.bind_vertex_shader(renderer.get_cmd_list());
    m_model_shader.bind_pixel_shader(renderer.get_cmd_list());
    m_model_layout.bind(renderer.get_cmd_list());

    u32 stride = sizeof(Vertex::PosColorNormal);
    u32 offset = 0;

    m_model_vb.bind(renderer.get_cmd_list(), 0, 1, &stride, &offset);
    m_model_ib.bind(renderer.get_cmd_list(), DataFormat::R32_UINT, 0);

    m_model_cb.bind_to_pixel_shader(renderer.get_cmd_list(), 0, 1);
    m_model_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 0, 1);

    ConstantBuffer2D cb_data;
    for (auto& sprite : m_sprites)
    {
        SpriteData& sprite_data = sprite->get_sprite_data();

        JMatrix4x4 scale = DirectX::XMMatrixScaling(sprite_data.size.x, -sprite_data.size.y, 1.0f);
        JMatrix4x4 rotation = DirectX::XMMatrixRotationZ(sprite_data.rotation);
        JMatrix4x4 translation = DirectX::XMMatrixTranslation(sprite_data.position.x, sprite_data.position.y, 0.0f);
        JMatrix4x4 world = scale * rotation * translation;

        JMatrix4x4 view = DirectX::XMLoadFloat4x4(&m_camera->get_view());
        JMatrix4x4 proj = DirectX::XMLoadFloat4x4(&m_camera->get_proj());

        JMatrix4x4 wvp = world * view * proj;

        XMStoreFloat4x4(&cb_data.world, XMMatrixTranspose(wvp));
        cb_data.color = sprite_data.color;
        cb_data.uv_rect = sprite_data.uv_rect;
        m_model_cb.update(renderer.get_cmd_list(), cb_data);

        renderer.get_cmd_list().device_context->PSSetShaderResources(0, 1, &sprite_data.texture.srv);

        sprite->draw();
        renderer.draw_sprite(sprite_data);
    }
}

void joj::D3D11Scene3D::shutdown()
{
}

void joj::D3D11Scene3D::draw_collisions(IRenderer& renderer)
{
}

#endif // JOJ_PLATFORM_WINDOWS