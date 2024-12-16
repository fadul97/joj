#include "hello_triangle.h"

#include "logger.h"
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "joj/math/jmath.h"

#include <fstream>
struct ModelType
{
    f32 x, y, z;
    f32 tu, tv;
    f32 nx, ny, nz;
};
ModelType* model = nullptr;
u32 vertexCount = 0;
i32 indexCount = 0;

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
ID3D11Buffer* m_indexBuffer = nullptr;

// Vertex Shader (HLSL)
const char* gVertexShaderCode = R"(
    cbuffer ConstantBuffer : register(b0)
    {
        float4x4 wvp;
        float4x4 worldMatrix;
	    float4x4 viewMatrix;
	    float4x4 projectionMatrix;
    };

    struct VS_INPUT {
        float4 position : POSITION;
        float2 tex : TEXCOORD0;
    };

    struct PS_INPUT {
        float4 position : SV_POSITION;
        float2 tex : TEXCOORD0;
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

        return output;
    }
)";

// Pixel Shader (HLSL)
const char* gPixelShaderCode = R"(
    struct PS_INPUT {
        float4 position : SV_POSITION;
        float2 tex : TEXCOORD0;
    };

    Texture2D shaderTexture : register(t0);
    SamplerState SampleType : register(s0);

    float4 main(PS_INPUT input) : SV_TARGET {
        float4 textureColor;

        // Sample the pixel color from the texture using the sampler at this texture coordinate location.
        textureColor = shaderTexture.Sample(SampleType, input.tex);

        return textureColor;
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

const int NUM_LIGHTS = 4;

struct LightPositionBuffer
{
    joj::JFloat4 lightPosition[NUM_LIGHTS];
};

struct LightBuffer
{
    joj::JFloat4 diffuseColor[NUM_LIGHTS];
};

b8 HelloTriangle::load_model(char* filename)
{
    std::ifstream fin;
    char input;
    int i;


    // Open the model file.
    fin.open(filename);

    // If it could not open the file then exit.
    if (fin.fail())
    {
        return false;
    }

    // Read up to the value of vertex count.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }

    // Read in the vertex count.
    fin >> vertexCount;

    // Set the number of indices to be the same as the vertex count.
    indexCount = vertexCount;

    // Create the model using the vertex count that was read in.
    model = new ModelType[vertexCount];

    // Read up to the beginning of the data.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // Read in the vertex data.
    for (i = 0; i < vertexCount; i++)
    {
        fin >> model[i].x >> model[i].y >> model[i].z;
        fin >> model[i].tu >> model[i].tv;
        fin >> model[i].nx >> model[i].ny >> model[i].nz;
    }

    // Close the model file.
    fin.close();

    return true;
}

void HelloTriangle::release_model()
{
    if (model)
    {
        delete[] model;
        model = nullptr;
    }
}

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
    m_cam.set_pos(0.0f, 0.0f, -10.0f);
    m_cam.update_view_matrix();
    // m_cam.set_lens(0.25f * J_PI, 800.0f / 600.0f, 0.1f, 1000.0f);
    // m_cam.update_view_matrix();
    // m_cam.look_at(m_cam.get_pos(), joj::JFloat3(0.0f, 0.0f, 0.0f), m_cam.get_up());
    // m_cam.update_view_matrix();

    timer.start();

    // Initialize the previous rendering position to negative one.
    m_prevPosX = -1;
    m_prevPosY = -1;

    // Set the number of vertices in the vertex array.
    vertexCount = 6;

    // Set the number of indices in the index array.
    indexCount = vertexCount;

    // Create the vertex array.
    Vertex* vertices = new Vertex[vertexCount];

    // Create the index array.
    u32* indices = new u32[indexCount];

    // Initialize vertex array to zeros at first.
    memset(vertices, 0, (sizeof(Vertex) * vertexCount));

    for (i32 i = 0; i < vertexCount; i++)
    {
        indices[i] = i;
    }

    // Set up the description of the static index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(u32) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    HRESULT result = renderer.get_device().device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return;
    }

    m_2dvb.setup(joj::BufferUsage::Dynamic, joj::CPUAccessType::Write,
        sizeof(Vertex) * vertexCount, vertices);
    m_2dvb.create(renderer.get_device());

    m_mat_cb.setup(joj::calculate_cb_byte_size(sizeof(ConstantBuffer)), nullptr);
    m_mat_cb.create(renderer.get_device());

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
    };

    renderer.get_device().device->CreateInputLayout(layout, 2,
        m_shader.get_vertex_shader().vsblob->GetBufferPointer(),
        m_shader.get_vertex_shader().vsblob->GetBufferSize(), &gInputLayout);

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;
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
        joj::JMatrix4x4 W = joj::matrix4x4_identity(); // XMMatrixIdentity();
        joj::JMatrix4x4 V = XMLoadFloat4x4(&m_cam.get_view());
        joj::JMatrix4x4 P;
        // P = XMLoadFloat4x4(&m_cam.get_proj());
        P = XMMatrixOrthographicLH(800.0f, 600.0f, 0.3f, 1000.0f);
        joj::JMatrix4x4 WVP = W * V * P;

        ConstantBuffer cbData = {};
        XMStoreFloat4x4(&cbData.wvp, XMMatrixTranspose(WVP));
        XMStoreFloat4x4(&cbData.worldMatrix, XMMatrixTranspose(W));
        XMStoreFloat4x4(&cbData.viewMatrix, XMMatrixTranspose(V));
        XMStoreFloat4x4(&cbData.projectionMatrix, XMMatrixTranspose(P));
        m_mat_cb.update(renderer.get_cmd_list(), cbData);
    }

    if ((m_prevPosX == m_renderX) && (m_prevPosY == m_renderY))
    {
        return;
    }
    else
    {
        m_prevPosX = m_renderX;
        m_prevPosY = m_renderY;

        Vertex* vertices = new Vertex[vertexCount];

        // Magic Numbers - Tutorial uses TGA files. The Converted PNG is larger,
        // this is a hacky fix for the screen coordinates
        f32 image_width = 512.0f;
        f32 image_height = 512.0f;

        f32 left, right, top, bottom;
        // Calculate the screen coordinates of the left side of the bitmap.
        left = (f32)((800 / 2) * -1) + (f32)m_renderX;

        // Calculate the screen coordinates of the right side of the bitmap.
        right = left + image_width;

        // Calculate the screen coordinates of the top of the bitmap.
        top = (f32)(600 / 2) - (f32)m_renderY;

        // Calculate the screen coordinates of the bottom of the bitmap.
        bottom = top - image_height;

        vertices[0].position = joj::JFloat3(left, top, 0.0f);  // Top left.
        vertices[0].texture = joj::JFloat2(0.0f, 0.0f);

        vertices[1].position = joj::JFloat3(right, bottom, 0.0f);  // Bottom right.
        vertices[1].texture = joj::JFloat2(1.0f, 1.0f);

        vertices[2].position = joj::JFloat3(left, bottom, 0.0f);  // Bottom left.
        vertices[2].texture = joj::JFloat2(0.0f, 1.0f);

        // Second triangle.
        vertices[3].position = joj::JFloat3(left, top, 0.0f);  // Top left.
        vertices[3].texture = joj::JFloat2(0.0f, 0.0f);

        vertices[4].position = joj::JFloat3(right, top, 0.0f);  // Top right.
        vertices[4].texture = joj::JFloat2(1.0f, 0.0f);

        vertices[5].position = joj::JFloat3(right, bottom, 0.0f);  // Bottom right.
        vertices[5].texture = joj::JFloat2(1.0f, 1.0f);

        D3D11_MAPPED_SUBRESOURCE mappedData;
        if (renderer.get_cmd_list().device_context->Map(
            m_2dvb.get_data().vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD,
            0, &mappedData) != S_OK)
        {
            JERROR(joj::ErrorCode::FAILED, "Failed to map Subresource data.");
            return;
        }

        Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);
        memcpy(v, (void*)vertices, (sizeof(Vertex) * vertexCount));

        renderer.get_cmd_list().device_context->Unmap(m_2dvb.get_data().vertex_buffer, 0);

        delete[] vertices;
        vertices = nullptr;
    }
}

void HelloTriangle::draw()
{
    renderer.clear();

    // Configurar o pipeline gráfico
    renderer.get_cmd_list().device_context->IASetInputLayout(gInputLayout);
    renderer.disable_depth_test();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_2dvb.bind(renderer.get_cmd_list(), 0, 1, &stride, &offset);
    renderer.get_cmd_list().device_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, offset);
    renderer.get_cmd_list().device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_mat_cb.bind_to_vertex_shader(renderer.get_cmd_list(), 0, 1);

    m_tex.bind(renderer.get_cmd_list(), 0, 1);
    renderer.get_cmd_list().device_context->PSSetSamplers(0, 1, &m_sampleState);

    m_shader.bind_vertex_shader(renderer.get_cmd_list());
    m_shader.bind_pixel_shader(renderer.get_cmd_list());

    // Desenhar o triângulo
    renderer.get_cmd_list().device_context->DrawIndexed(indexCount, 0, 0);

    renderer.enable_depth_test();

    renderer.swap_buffers();
}

void HelloTriangle::shutdown()
{
    timer.end_period();

    release_model();

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