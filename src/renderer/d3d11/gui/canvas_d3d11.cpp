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
            JFloat3{ m_normalized_x, m_normalized_y, 0.0f }, // Top Left
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ m_normalized_x + m_normalized_width, m_normalized_y, 0.0f }, // Top Right
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ m_normalized_x + m_normalized_width, m_normalized_y - m_normalized_height, 0.0f }, // Bottom Right
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
        {
            JFloat3{ m_normalized_x, m_normalized_y - m_normalized_height, 0.0f }, // Bottom Left
            JFloat4{ m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a }
        },
    };

    JDEBUG("Canvas size: %f, %f, %f, %f",
        m_normalized_x, m_normalized_y, m_normalized_width, m_normalized_height);

    JDEBUG("[Top Left]: %f, %f, %f",
        quad_vertices[0].pos.x, quad_vertices[0].pos.y, quad_vertices[0].pos.z);

    JDEBUG("[Top Right]: %f, %f, %f",
        quad_vertices[1].pos.x, quad_vertices[1].pos.y, quad_vertices[1].pos.z);

    JDEBUG("[Bottom Left]: %f, %f, %f",
        quad_vertices[3].pos.x, quad_vertices[3].pos.y, quad_vertices[3].pos.z);

    JDEBUG("[Bottom Right]: %f, %f, %f",
        quad_vertices[2].pos.x, quad_vertices[2].pos.y, quad_vertices[2].pos.z);

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

    // Compile and Create Shader
    m_shader.compile_vertex_shader_from_file("shaders/Canvas.hlsl", "VS", ShaderModel::Default);
    m_shader.compile_pixel_shader_from_file("shaders/Canvas.hlsl", "PS", ShaderModel::Default);
    JOJ_LOG_IF_FAIL(m_shader.create_vertex_shader(device));
    JOJ_LOG_IF_FAIL(m_shader.create_pixel_shader(device));

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