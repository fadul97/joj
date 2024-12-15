#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "joj/math/jmath.h"

using namespace DirectX;

// Estrutura para um vértice com posição e cor
struct Vertex
{
    joj::JFloat3 position;
    joj::JFloat2 texture;
    joj::JFloat3 normal;
};

ID3D11InputLayout* gInputLayout = nullptr;
ID3D11SamplerState* m_sampleState = nullptr;

// Vertex Shader (HLSL)
const char* gVertexShaderCode = R"(
    cbuffer ConstantBuffer : register(b0) {
        float4x4 wvp;
        float4x4 worldMatrix;
	    float4x4 viewMatrix;
	    float4x4 projectionMatrix;
    };

    struct VS_INPUT {
        float4 position : POSITION;
        float2 tex : TEXCOORD0;
        float3 normal : NORMAL;
    };

    struct PS_INPUT {
        float4 position : SV_POSITION;
        float2 tex : TEXCOORD0;
        float3 normal : NORMAL;
    };

    PS_INPUT main(VS_INPUT input) {
        PS_INPUT output;
        
        // Change the position vector to be 4 units for proper matrix calculations.
        input.position.w = 1.0f;

        // Calculate the position of the vertex against the world, view, and projection matrices.
        output.position = mul(input.position, worldMatrix);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        output.tex = input.tex;

        // Calculate the normal vector against the world matrix only.
        output.normal = mul(input.normal, (float3x3)worldMatrix);

        // Normalize the normal vector.
        output.normal = normalize(output.normal);

        return output;
    }
)";

// Pixel Shader (HLSL)
const char* gPixelShaderCode = R"(
    cbuffer LightBuffer : register(b0)
    {
        float4 diffuseColor;
        float3 lightDirection;
        float padding;
    };

    struct PS_INPUT {
        float4 position : SV_POSITION;
        float2 tex : TEXCOORD0;
        float3 normal : NORMAL;
    };

    Texture2D shaderTexture : register(t0);
    SamplerState SampleType : register(s0);

    float4 main(PS_INPUT input) : SV_TARGET {
        float4 textureColor;
        float3 lightDir;
        float lightIntensity;
        float4 color;

        // Sample the pixel color from the texture using the sampler at this texture coordinate location.
        textureColor = shaderTexture.Sample(SampleType, input.tex);
        
        // Invert the light direction for calculations.
        lightDir = -lightDirection;

        // Calculate the amount of light on this pixel.
        lightIntensity = saturate(dot(input.normal, lightDir));

        // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
        color = saturate(diffuseColor * lightIntensity);

        // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
        color = color * textureColor;

        return color;
    }
)";

// Estrutura para o Constant Buffer
struct ConstantBuffer
{
    joj::JFloat4x4 wvp;
    joj::JFloat4x4 worldMatrix;
    joj::JFloat4x4 viewMatrix;
    joj::JFloat4x4 projectionMatrix;
};

struct LightBuffer
{
    joj::JFloat4 diffuseColor;
    joj::JFloat3 lightDirection;
    f32 padding;
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

    m_cam.set_pos(0.0f, 0.0f, -3.0f);
    m_cam.update_view_matrix();
    m_cam.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    m_cam.update_view_matrix();

    timer.start();

    // Criação do Vertex Buffer
    Vertex vertices[] = {
        { joj::JFloat3(0.0f,  0.5f, 0.0f), joj::JFloat2(0.5f, 0.0f), joj::JFloat3{0.0f, 0.0f, -1.0f} },  // Top
        { joj::JFloat3(0.5f, -0.5f, 0.0f), joj::JFloat2(1.0f, 1.0f), joj::JFloat3{0.0f, 0.0f, -1.0f} },  // Bottom right
        { joj::JFloat3(-0.5f, -0.5f, 0.0f), joj::JFloat2(0.0f, 1.0f), joj::JFloat3{0.0f, 0.0f, -1.0f} }  // Bottom left
    };

    m_vb.setup(joj::BufferUsage::Default, joj::CPUAccessType::None,
        sizeof(vertices), vertices);
    m_vb.create(renderer.get_device());

    m_mat_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
    m_mat_cb.create(renderer.get_device());

    m_light_cb.setup(joj::calculate_cb_byte_size(sizeof(LightBuffer)), nullptr);
    m_light_cb.create(renderer.get_device());

    m_shader.compile_vertex_shader(gVertexShaderCode, "main", joj::ShaderModel::Default);
    m_shader.create_vertex_shader(renderer.get_device());

    m_shader.compile_pixel_shader(gPixelShaderCode, "main", joj::ShaderModel::Default);
    m_shader.create_pixel_shader(renderer.get_device());

    m_tex.create(renderer.get_device(),
        renderer.get_cmd_list(),
        L"../../../../samples/textures/stone.png",
        joj::ImageType::PNG);

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    if (renderer.get_device().device->CreateSamplerState(&samplerDesc, &m_sampleState) != S_OK)
    {
        JERROR(joj::ErrorCode::FAILED, "Failed to create D3D11 Sampler State.");
    }

    // Layout de entrada
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    renderer.get_device().device->CreateInputLayout(layout, 3,
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
        joj::JMatrix4x4 W = XMMatrixRotationY(rotation); // XMMatrixIdentity();
        joj::JMatrix4x4 V = XMLoadFloat4x4(&m_cam.get_view());
        joj::JMatrix4x4 P = XMLoadFloat4x4(&m_cam.get_proj());
        joj::JMatrix4x4 WVP = W * V * P;

        ConstantBuffer cbData = {};
        XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(W));

        joj::JMatrix4x4 wvp = XMMatrixTranspose(WVP);
        XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(wvp));

        joj::JMatrix4x4 I = XMMatrixIdentity();
        XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(I));
        XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(wvp));
        XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(wvp));
        m_mat_cb.update(renderer.get_cmd_list(), cbData);
    }

    {
        LightBuffer lightBuffer;
        lightBuffer.diffuseColor = joj::JFloat4(1.0f, 0.7f, 0.7f, 1.0);
        lightBuffer.lightDirection = joj::JFloat3(0.0f, 0.0f, 1.0f);
        m_light_cb.update(renderer.get_cmd_list(), lightBuffer);
    }
}

void HelloTriangle::draw()
{
    renderer.clear();

    // Configurar o pipeline gráfico
    renderer.get_cmd_list().device_context->IASetInputLayout(gInputLayout);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_vb.bind(renderer.get_cmd_list(), 0, 1, &stride, &offset);
    renderer.get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_mat_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 0, 1);
    m_light_cb.bind_to_pixel_shader(renderer.get_cmd_list(), 0, 1);

    m_tex.bind(renderer.get_cmd_list(), 0, 1);
    renderer.get_cmd_list().device_context->PSSetSamplers(0, 1, &m_sampleState);

    m_shader.bind_vertex_shader(renderer.get_cmd_list());
    m_shader.bind_pixel_shader(renderer.get_cmd_list());

    // Desenhar o triângulo
    renderer.get_cmd_list().device_context->Draw(3, 0);

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();

    m_sampleState->Release();
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