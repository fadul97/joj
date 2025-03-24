#include "app3d_test.h"

#include "joj/core/logger.h"
#include "joj/core/jmacros.h"
#include "joj/renderer/vertex.h"
#include "joj/engine/engine.h"
#include <math.h>
#include "joj/math/jvector3.h"
#include "joj/joj.h"

// Read file
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // rand()
#include <unordered_map>

joj::JFloat3 cross(const joj::JFloat3& v1, const joj::JFloat3& v2) {
    return joj::JFloat3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

joj::JFloat3 normalize(const joj::JFloat3& v) {
    f32 len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.0f)
        return joj::JFloat3(v.x / len, v.y / len, v.z / len);

    return joj::JFloat3(0.0f, 0.0f, 0.0f); // Retorna um vetor nulo se o comprimento for zero
}

joj::JFloat3 calculateFaceNormal(const joj::JFloat3& v0, const joj::JFloat3& v1, const joj::JFloat3& v2)
{
    joj::JFloat3 edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
    joj::JFloat3 edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
    return normalize(cross(edge1, edge2));
}

App3DTest::App3DTest()
{
    m_last_mouse_pos = { 0, 0 };
    joj_print();
}

App3DTest::~App3DTest()
{
}

void App3DTest::setup_camera()
{
    m_camera.update_view_matrix();
    m_camera.set_pos(0.0f, 5.0f, -15.0f);
    m_camera.update_view_matrix();
    m_camera.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_camera.update_view_matrix();
    m_camera.look_at(m_camera.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_camera.get_up());
    m_camera.update_view_matrix();
}

void App3DTest::build_shader()
{
    JOJ_INFO("Building shader...");

    m_shader = joj::D3D11Shader(m_renderer->get_device(), m_renderer->get_cmd_list());
    m_shader.compile_vertex_shader_from_file(
        "shaders/SimpleModel.hlsl",
        "VS", joj::ShaderModel::Default);

    m_shader.compile_pixel_shader_from_file(
        "shaders/SimpleModel.hlsl",
        "PS", joj::ShaderModel::Default);
}

void App3DTest::build_input_layout()
{
    JOJ_INFO("Building input layout...");

    std::vector<joj::InputDesc> sprite_layout = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT,    0,  0, joj::InputClassification::PerVertexData, 0 },
        { "COLOR",    0, joj::DataFormat::R32G32B32A32_FLOAT, 0, 12, joj::InputClassification::PerVertexData, 0 },
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT,    0, 28, joj::InputClassification::PerVertexData, 0 }
    };

    m_shader.create_input_layout(sprite_layout);
}

static u32 total_indices = 0;
void App3DTest::build_buffers()
{
    JOJ_INFO("Building buffers... NOT");
}

void App3DTest::init()
{
    setup_camera();
    build_shader();
    build_input_layout();
    build_buffers();
    m_renderer->set_rasterizer_state(joj::RasterizerState::Solid);
}

void App3DTest::update(const f32 dt)
{
    if (m_input->is_key_pressed(joj::KEY_ESCAPE))
        joj::Engine::close();

    const f32 speed = 10.0f * dt;;

    process_mouse_input(dt);
}

static f32 angleY = 0.0f;
void App3DTest::draw()
{
    m_renderer->begin_frame();
    {
        m_shader.bind();
        
        static f32 rotation = 0.0f;
        // Update the rotation variable each frame.
        rotation -= 0.0174532925f * 0.1f;
        if (rotation < 0.0f)
        rotation += 360.0f;
        
        static f32 angle = 0.0f;
        angle += 0.01f;
        {
            m_constant_buffer.bind_to_vertex_shader(0, 1);
            joj::JMatrix4x4 W = DirectX::XMMatrixRotationY(rotation);
            joj::JMatrix4x4 V = DirectX::XMLoadFloat4x4(&m_camera.get_view());
            joj::JMatrix4x4 P = DirectX::XMLoadFloat4x4(&m_camera.get_proj());
            joj::JMatrix4x4 WVP = W * V * P;
            
            ConstantBuffer cb{};
            XMStoreFloat4x4(&cb.worldMatrix, XMMatrixTranspose(W));
            XMStoreFloat4x4(&cb.wvp, XMMatrixTranspose(WVP));
            XMStoreFloat4x4(&cb.viewMatrix, XMMatrixTranspose(V));
            XMStoreFloat4x4(&cb.projectionMatrix, XMMatrixTranspose(P));
            m_constant_buffer.update(cb);
            
            m_light_buffer.bind_to_pixel_shader(1, 1);
            LightCB lightBuffer{};
            lightBuffer.diffuseColor = joj::Vector4(0.7f, 0.7f, 0.7f, 1.0);
            lightBuffer.lightDirection = joj::Vector3(0.0f, 0.0f, 1.0f);
            m_light_buffer.update(lightBuffer);
        }
        
        m_renderer->draw_indexed(total_indices, 0, 0);
    }
    m_renderer->end_frame();
}

void App3DTest::shutdown()
{
}

void App3DTest::on_mouse_down(joj::Buttons button, i32 x, i32 y)
{
    m_last_mouse_pos.x = x;
    m_last_mouse_pos.y = y;

    SetCapture(m_window->get_data()->handle);
}

void App3DTest::on_mouse_up(joj::Buttons button, i32 x, i32 y)
{
    ReleaseCapture();
}

void App3DTest::on_mouse_move(WPARAM button_state, i32 x, i32 y)
{
    if ((button_state & MK_RBUTTON) != 0)
    {
        f32 dx = DirectX::XMConvertToRadians(0.25f * static_cast<f32>(x - m_last_mouse_pos.x));
        f32 dy = DirectX::XMConvertToRadians(0.25f * static_cast<f32>(y - m_last_mouse_pos.y));

        m_camera.pitch(dy);
        m_camera.rotateY(dx);
    }

    m_last_mouse_pos.x = x;
    m_last_mouse_pos.y = y;
}

void App3DTest::process_mouse_input(const f32 dt)
{
    const f32 speed = dt * 20.0f;

    if (m_input->is_key_down('W'))
        m_camera.walk(speed);

    if (m_input->is_key_down(joj::KEY_S))
        m_camera.walk(-speed);

    if (m_input->is_key_down(joj::KEY_A))
        m_camera.strafe(-speed);

    if (m_input->is_key_down(joj::KEY_D))
        m_camera.strafe(speed);

    m_camera.update_view_matrix();
}