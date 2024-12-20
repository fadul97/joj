#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "joj/math/jmath.h"
#include "joj/jmacros.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace DirectX;

struct CameraBufferType
{
    joj::JFloat3 cameraPosition;
    f32 padding;
};

struct LightBuffer
{
    joj::JFloat4 ambientColor;
    joj::JFloat4 diffuseColor;
    joj::JFloat3 lightDirection;
    f32 specularPower;
    joj::JFloat4 specularColor;
};

HelloTriangle::HelloTriangle()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    input = joj::Win32Input();
    timer = joj::Win32Timer();
    renderer = joj::D3D11Renderer();
}

HelloTriangle::~HelloTriangle()
{

}

void HelloTriangle::init()
{
    if (window.create() != joj::ErrorCode::OK)
        return;

    u32 width = 0;
    u32 height = 0;

    window.get_window_size(width, height);
    JDEBUG("Window size: %dx%d", width, height);

    window.get_client_size(width, height);
    JDEBUG("Client size: %dx%d", width, height);

    input.set_window(window.get_data());

    timer.begin_period();

    renderer_print();

    if (renderer.initialize(window.get_data()) != joj::ErrorCode::OK)
        return;

    m_cam.update_view_matrix();
    m_cam.set_pos(0.0f, 5.0f, -20.0f);
    m_cam.update_view_matrix();
    m_cam.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_cam.update_view_matrix();
    m_cam.look_at(m_cam.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_cam.get_up());
    m_cam.update_view_matrix();

    timer.start();

    m_spaceship = joj::D3D11Mesh("../../../../samples/models/MySpaceShip.obj", joj::MeshType::OBJ);
    m_spaceship.setup(renderer.get_device());
    m_spaceship.translate(-10, 0, 10);

    m_spaceship2 = joj::D3D11Mesh("../../../../samples/models/MyCube.obj", joj::MeshType::OBJ);
    m_spaceship2.setup(renderer.get_device());
    m_spaceship2.translate(10, 0, 10);

    m_light_cb.setup(joj::calculate_cb_byte_size(sizeof(LightBuffer)), nullptr);
    JOJ_LOG_IF_FAIL(m_light_cb.create(renderer.get_device()));

    m_camera_cb.setup(joj::calculate_cb_byte_size(sizeof(CameraBufferType)), nullptr);
    JOJ_LOG_IF_FAIL(m_camera_cb.create(renderer.get_device()));

    // Describe a texture sampler.
    joj::SamplerDesc samplerDesc;
    samplerDesc.filter = joj::SamplerFilter::MIN_MAG_MIP_LINEAR;
    samplerDesc.addressU = joj::TextureAddressMode::Wrap;
    samplerDesc.addressV = joj::TextureAddressMode::Wrap;
    samplerDesc.addressW = joj::TextureAddressMode::Wrap;
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
    JOJ_LOG_IF_FAIL(m_sampler_state.create(renderer.get_device(), samplerDesc));
    m_sampler_state.bind(renderer.get_cmd_list(), joj::SamplerType::Anisotropic, 0, 1);

    joj::InputDesc layout[] = {
        { "POSITION", 0, joj::DataFormat::R32G32B32_FLOAT, 0,  0, joj::InputClassification::PerVertexData, 0 },
        { "TEXCOORD", 0, joj::DataFormat::R32G32_FLOAT,    0, 12, joj::InputClassification::PerVertexData, 0 },
        { "NORMAL",   0, joj::DataFormat::R32G32B32_FLOAT, 0, 20, joj::InputClassification::PerVertexData, 0 },
    };

    for (auto& l : layout)
    {
        m_input_layout.add(l);
    }

    JOJ_LOG_IF_FAIL(m_input_layout.create(renderer.get_device(), m_spaceship.get_shader().get_vertex_shader()));
}

void HelloTriangle::update(const f32 dt)
{
    if (input.is_key_pressed('A'))
        JDEBUG("A pressed.");

    if (input.is_key_down(joj::KEY_SPACE))
        JDEBUG("Space down.");

    if (input.is_key_pressed(joj::KEY_ESCAPE))
        loop = false;


    static float rotation = 0.0f;
    // Update the rotation variable each frame.
    rotation -= 0.0174532925f * 0.1f;
    if (rotation < 0.0f)
    {
        rotation += 360.0f;
    }

    static float angle = 0.0f;
    angle += 0.01f;

    {
        m_spaceship.update(m_cam.get_view(), m_cam.get_proj(), dt);
        m_spaceship2.update(m_cam.get_view(), m_cam.get_proj(), dt);
    }

    {
        LightBuffer lightBuffer;
        lightBuffer.ambientColor = joj::JFloat4(0.15f, 0.15f, 0.15f, 1.0f);
        lightBuffer.diffuseColor = joj::JFloat4(1.0f, 1.0f, 1.0f, 1.0);
        lightBuffer.lightDirection = joj::JFloat3(1.0f, -1.0f, 1.0f);
        lightBuffer.specularColor = joj::JFloat4(0.0f, 0.0f, 1.0f, 1.0f);
        lightBuffer.specularPower = 32.0f;
        m_light_cb.update(renderer.get_cmd_list(), lightBuffer);
    }

    {
        CameraBufferType cameraBuffer;
        cameraBuffer.cameraPosition = m_cam.get_pos();
        m_camera_cb.update(renderer.get_cmd_list(), cameraBuffer);
    }
}

void HelloTriangle::draw()
{
    renderer.clear();

    m_input_layout.bind(renderer.get_cmd_list());
    renderer.set_primitive_topology(joj::PrimitiveTopology::TRIANGLE_LIST);
    
    m_light_cb.bind_to_pixel_shader(renderer.get_cmd_list(), 0, 1);
    m_camera_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 1, 1);

    m_spaceship.draw(renderer.get_device(), renderer.get_cmd_list());
    m_spaceship2.draw(renderer.get_device(), renderer.get_cmd_list());

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();
}

f32 HelloTriangle::get_frametime()
{
#ifdef JOJ_DEBUG_MODE
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter
#endif // JOJ_DEBUG_MODE

    // Current frame time
    frametime = timer.reset();

#ifdef JOJ_DEBUG_MODE
    // Accumulated frametime
    total_time += frametime;

    // Increment frame counter
    frame_count++;

    // Updates FPS indicator in the window every 1000ms (1 second)
    if (total_time >= 1.0f)
    {
        std::stringstream text;		// Text flow for messages
        text << std::fixed;			// Always show the fractional part
        text.precision(3);			// three numbers after comma

        text << "Joj Engine v0.0.1" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << frametime * 1000 << " (ms)";

        window.set_title(text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
#endif // JOJ_DEBUG_MODE

    return frametime;
}