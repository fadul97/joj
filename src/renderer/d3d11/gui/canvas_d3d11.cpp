#include "renderer/d3d11/gui/canvas_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "logger.h"
#include "renderer/d3d11/shader_library_d3d11.h"
#include "jmacros.h"
#include "renderer/vertex.h"

joj::D3D11Canvas::D3D11Canvas()
{
}

joj::D3D11Canvas::D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height)
    : Canvas(x, y, width, height)
{
}

joj::D3D11Canvas::D3D11Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color)
    : Canvas(x, y, width, height, color)
{
}

joj::D3D11Canvas::~D3D11Canvas()
{
}

joj::ErrorCode joj::D3D11Canvas::create(GraphicsDevice& device)
{
    /*
    struct PosColor
	{
        JFloat3 pos;
        JFloat4 color;
	};
    */
    const Vertex::PosColor quad_vertices[4] =
    {
        // Position
        {
            JFloat3{ static_cast<f32>(m_x), static_cast<f32>(m_y), 0.0f }, // Top Left
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ static_cast<f32>(m_x + m_width), static_cast<f32>(m_y), 0.0f }, // Top Right
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ static_cast<f32>(m_x + m_width), static_cast<f32>(m_y + m_height), 0.0f }, // Bottom Right
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ static_cast<f32>(m_x), static_cast<f32>(m_y + m_height), 0.0f }, // Bottom Left
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
    };

    m_vb.setup(BufferUsage::Immutable, CPUAccessType::None,
        sizeof(Vertex::PosColor) * 4, quad_vertices);

    // Create vertex buffer
    if JOJ_FAILED(m_vb.create(device))
    {
        JERROR(ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION,
            "Failed to create D3D11 Vertex Buffer for D3D11Canvas.");
        return ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION;
    }

    const u32 quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_ib.setup(sizeof(u32) * 6, quad_indices);

    // Create index buffer
    if JOJ_FAILED(m_ib.create(device))
    {
        JERROR(ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION,
            "Failed to create D3D11 index buffer.");
        return ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION;
    }

    // Create shader
    m_shader.compile_vertex_shader(D3D11ShaderLibrary::VertexShaderCanvas, "VS", ShaderModel::Default);
    m_shader.compile_pixel_shader(D3D11ShaderLibrary::PixelShaderCanvas, "PS", ShaderModel::Default);

    // Create input layout
    InputDesc input_desc[] =
    {
        { "POSITION", 0, DataFormat::R32G32B32_FLOAT,    0,  0, InputClassification::PerVertexData, 0 },
        { "COLOR",    0, DataFormat::R32G32B32A32_FLOAT, 0, 12, InputClassification::PerVertexData, 0 },
    };

    for (auto& desc : input_desc)
        m_input_layout.add(desc);

    if JOJ_FAILED(m_input_layout.create(device, m_shader.get_vertex_shader()))
    {
        JERROR(ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION,
            "Failed to create D3D11 Input Layout for D3D11Canvas.");
        return ErrorCode::ERR_GUI_CANVAS_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11Canvas::draw(CommandList& cmd_list)
{
    u32 stride = sizeof(Vertex::PosColor);
    u32 offset = 0;
    m_vb.bind(cmd_list, 0, 1, &stride, &offset);
    m_ib.bind(cmd_list, DataFormat::R32_UINT, offset);
    m_input_layout.bind(cmd_list);

    m_shader.bind_vertex_shader(cmd_list);
    m_shader.bind_pixel_shader(cmd_list);
    
    cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmd_list.device_context->DrawIndexed(6, 0, 0);
}

#endif // JPLATFORM_WINDOWS