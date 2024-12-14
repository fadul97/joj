#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX;

// Estrutura para um v�rtice com posi��o e cor
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
};

ID3D11InputLayout* gInputLayout = nullptr;

// Vertex Shader (HLSL)
const char* gVertexShaderCode = R"(
    cbuffer ConstantBuffer : register(b0) {
        float4x4 transform;
    };

    struct VS_INPUT {
        float3 position : POSITION;
        float4 color : COLOR;
    };

    struct PS_INPUT {
        float4 position : SV_POSITION;
        float4 color : COLOR;
    };

    PS_INPUT main(VS_INPUT input) {
        PS_INPUT output;
        output.position = mul(float4(input.position, 1.0f), transform);
        output.color = input.color;
        return output;
    }
)";

// Pixel Shader (HLSL)
const char* gPixelShaderCode = R"(
    struct PS_INPUT {
        float4 position : SV_POSITION;
        float4 color : COLOR;
    };

    float4 main(PS_INPUT input) : SV_TARGET {
        return input.color;
    }
)";

// Estrutura para o Constant Buffer
struct ConstantBuffer {
    XMMATRIX transform;
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

    timer.start();

    // Cria��o do Vertex Buffer
    Vertex vertices[] = {
        { XMFLOAT3(0.0f,  0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    m_vb.setup(joj::BufferUsage::Default, joj::CPUAccessType::None,
        sizeof(vertices), vertices);
    m_vb.create(renderer.get_device());

    m_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
    m_cb.create(renderer.get_device());

    m_shader.compile_vertex_shader(gVertexShaderCode, "main", joj::ShaderModel::Default);
    m_shader.create_vertex_shader(renderer.get_device());

    m_shader.compile_pixel_shader(gPixelShaderCode, "main", joj::ShaderModel::Default);
    m_shader.create_pixel_shader(renderer.get_device());

    // Layout de entrada
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    renderer.get_device().device->CreateInputLayout(layout, 2,
        m_shader.get_vertex_shader().vsblob->GetBufferPointer(),
        m_shader.get_vertex_shader().vsblob->GetBufferSize(), &gInputLayout);
}

void HelloTriangle::update(const f32 dt)
{
    if (input.is_key_pressed('A'))
        JDEBUG("A pressed.");

    if (input.is_key_down(joj::KEY_SPACE))
        JDEBUG("Space down.");

    if (input.is_key_pressed(joj::KEY_ESCAPE))
        loop = false;

    static float angle = 0.0f;
    angle += 0.01f;

    XMMATRIX rotationMatrix = XMMatrixRotationZ(angle);
    ConstantBuffer cbData = {};
    cbData.transform = XMMatrixTranspose(rotationMatrix); // Transpor para o formato row-major exigido pelo HLSL
    cbData.transform = XMMatrixTranspose(XMMatrixIdentity());
    m_cb.update(renderer.get_cmd_list(), cbData);
}

void HelloTriangle::draw()
{
    renderer.clear();

    // Configurar o pipeline gr�fico
    renderer.get_cmd_list().device_context->IASetInputLayout(gInputLayout);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_vb.bind(renderer.get_cmd_list(), 0, 1, &stride, &offset);
    renderer.get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 0, 1);

    m_shader.bind_vertex_shader(renderer.get_cmd_list());
    m_shader.bind_pixel_shader(renderer.get_cmd_list());

    // Desenhar o tri�ngulo
    renderer.get_cmd_list().device_context->Draw(3, 0);

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();

    gInputLayout->Release();
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