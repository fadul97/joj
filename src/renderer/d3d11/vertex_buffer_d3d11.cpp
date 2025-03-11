#include "renderer/d3d11/vertex_buffer_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "core/logger.h"
#include "core/jmacros.h"

joj::D3D11VertexBuffer::D3D11VertexBuffer()
{
    m_data.vbd = { 0 };
    m_data.vertex_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11VertexBuffer::~D3D11VertexBuffer()
{
    if (m_filled)
    {
        // Release Vertex Buffer
        if (m_data.vertex_buffer)
        {
            m_data.vertex_buffer->Release();
            m_data.vertex_buffer = nullptr;
        }
    }
}

void joj::D3D11VertexBuffer::setup(BufferUsage usage, CPUAccessType cpu_access_flags,
    u32 byte_width, const void* data)
{
    // Describe Buffer - Resource structure
    m_data.vbd.ByteWidth = byte_width;
    m_data.vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    switch (usage)
    {
    case BufferUsage::Default:
        m_data.vbd.Usage = D3D11_USAGE_DEFAULT;
        break;
    case BufferUsage::Immutable:
        m_data.vbd.Usage = D3D11_USAGE_IMMUTABLE;
        break;
    case BufferUsage::Dynamic:
        m_data.vbd.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case BufferUsage::Staging:
        m_data.vbd.Usage = D3D11_USAGE_STAGING;
        break;
    default:
        break;
    }

    switch (cpu_access_flags)
    {
    case CPUAccessType::None:
        m_data.vbd.CPUAccessFlags = 0;
        break;
    case CPUAccessType::Write:
        m_data.vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    case CPUAccessType::Read:
        m_data.vbd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
    default:
        break;
    }

    m_data.vbd.MiscFlags = 0;
    m_data.vbd.StructureByteStride = 0;

    if (data != nullptr)
        m_data.init_data.pSysMem = data;

    m_filled = true;
}

joj::ErrorCode joj::D3D11VertexBuffer::create(const GraphicsDevice& device)
{
    if (device.device->CreateBuffer(
        &m_data.vbd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.vertex_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_VERTEX_BUFFER_D3D11_CREATION,
            "Failed to create D3D11 Vertex Buffer.");
        return ErrorCode::ERR_VERTEX_BUFFER_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11VertexBuffer::bind(CommandList& cmd_list, u32 start_slot,
    u32 num_buffers, const u32* stride, const u32* offset)
{
    cmd_list.device_context->IASetVertexBuffers(start_slot, num_buffers, &m_data.vertex_buffer, stride, offset);
}

joj::VBData& joj::D3D11VertexBuffer::get_data()
{
    return m_data;
}

void joj::D3D11VertexBuffer::cleanup()
{
    if (m_filled)
    {
        // Release Vertex Buffer
        if (m_data.vertex_buffer)
        {
            m_data.vertex_buffer->Release();
            m_data.vertex_buffer = nullptr;
        }

        m_data.init_data.pSysMem = nullptr;
    }

    m_filled = false;
}

void joj::D3D11VertexBuffer::update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data)
{
    JOJ_ASSERT(data != nullptr);
    JOJ_ASSERT(cmd_list.device_context != nullptr);

    if (m_data.vbd.Usage != D3D11_USAGE_DYNAMIC)
    {
        JOJ_ERROR(ErrorCode::FAILED, "Vertex Buffer usage is not dynamic.");
        return;
    }

    if (m_data.vbd.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE)
    {
        JOJ_ERROR(ErrorCode::FAILED, "Vertex Buffer CPUAcessFlags is not writable.");
        return;
    }

    // Data to be copied
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
    mapped_buffer.pData = nullptr;

    // Get a pointer to the vertex buffer data and lock it
    if (cmd_list.device_context->Map(m_data.vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::FAILED, "Failed to map vertex buffer.");
        return;
    }
    else
    {
        // Copy the new data to the constant buffer data.
        memcpy(mapped_buffer.pData, data, sizeof_data);

        // Release the pointer to the constant buffer data.
        cmd_list.device_context->Unmap(m_data.vertex_buffer, 0);
    }
}

#endif // JOJ_PLATFORM_WINDOWS