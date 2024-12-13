#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX;

// Estrutura para um vértice com posição e cor
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
};

ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11InputLayout* gInputLayout = nullptr;
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;

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

ID3D11Buffer* gConstantBuffer = nullptr;

// Estrutura para o Constant Buffer
struct ConstantBuffer {
    XMMATRIX transform;
};

HelloTriangle::HelloTriangle()
{
    window = joj::Win32Window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    input = joj::Win32Input();
    timer = joj::Win32Timer();
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

    // renderer = joj::D3D11Renderer();
    if (renderer.initialize(window.get_data()) != joj::ErrorCode::OK)
        return;

    timer.start();

    // Criação do Vertex Buffer
    Vertex vertices[] = {
        { XMFLOAT3(0.0f,  0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    renderer.get_device().device->CreateBuffer(&bufferDesc, &initData, &gVertexBuffer);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;

    HRESULT hr = renderer.get_device().device->CreateBuffer(&cbDesc, nullptr, &gConstantBuffer);
    if (FAILED(hr)) {
        OutputDebugStringA("Falha ao criar Constant Buffer.\n");
        return;
    }

    // Compilação dos shaders e criação do pipeline
    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    hr = D3DCompile(
        gVertexShaderCode, strlen(gVertexShaderCode), nullptr, nullptr, nullptr,
        "main", "vs_5_0", 0, 0, &vertexShaderBlob, &errorBlob);

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return; // Falha ao compilar o shader
    }

    renderer.get_device().device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &gVertexShader);
    
    ID3DBlob* pixelShaderBlob = nullptr;
    hr = D3DCompile(
        gPixelShaderCode, strlen(gPixelShaderCode), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &pixelShaderBlob, &errorBlob);

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return; // Falha ao compilar o shader
    }

    renderer.get_device().device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &gPixelShader);
    pixelShaderBlob->Release();

    // Layout de entrada
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    renderer.get_device().device->CreateInputLayout(layout, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &gInputLayout);
    vertexShaderBlob->Release();
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
    cbData.transform = XMMatrixIdentity();
    renderer.get_cmd_list().device_context->UpdateSubresource(gConstantBuffer, 0, nullptr, &cbData, 0, 0);
}

void HelloTriangle::draw()
{
    renderer.clear();

    // Configurar o pipeline gráfico
    renderer.get_cmd_list().device_context->IASetInputLayout(gInputLayout);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    renderer.get_cmd_list().device_context->IASetVertexBuffers(0, 1, &gVertexBuffer, &stride, &offset);
    renderer.get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.get_cmd_list().device_context->VSSetConstantBuffers(0, 1, &gConstantBuffer);

    renderer.get_cmd_list().device_context->VSSetShader(gVertexShader, nullptr, 0);
    renderer.get_cmd_list().device_context->PSSetShader(gPixelShader, nullptr, 0);

    // Desenhar o triângulo
    renderer.get_cmd_list().device_context->Draw(3, 0);

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();

    gVertexBuffer->Release();
    gInputLayout->Release();
    gVertexShader->Release();
    gPixelShader->Release();
    gConstantBuffer->Release();
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