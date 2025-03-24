#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/jmacros.h"
#include "joj/renderer/d3d11/graphics_device_d3d11.h"
#include "joj/renderer/d3d11/command_list_d3d11.h"
#include "joj/core/logger.h"

joj::D3D11VertexBuffer::D3D11VertexBuffer()
    : m_device(nullptr), m_cmd_list(nullptr), m_filled(false)
{
    m_data.vbd = { 0 };
    m_data.vertex_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11VertexBuffer::D3D11VertexBuffer(GraphicsDevice* device, CommandList* cmd_list)
    : m_device(device), m_cmd_list(cmd_list), m_filled(false)
{
    m_data.vbd = { 0 };
    m_data.vertex_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11VertexBuffer::~D3D11VertexBuffer()
{
    cleanup();
}

joj::ErrorCode joj::D3D11VertexBuffer::create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
    const u32 byte_width, const void* data)
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

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
        m_data.vbd.Usage = D3D11_USAGE_DEFAULT;
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

    if (m_device->device->CreateBuffer(
        &m_data.vbd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.vertex_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_VERTEX_BUFFER_CREATION,
            "Failed to create D3D11 Vertex Buffer.");
        return ErrorCode::ERR_VERTEX_BUFFER_CREATION;
    }

    m_filled = true;

    return ErrorCode::OK;
}

void joj::D3D11VertexBuffer::bind(const u32 start_slot, const u32 num_buffers,
    const u32* stride, const u32* offset)
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");
    JOJ_ASSERT(m_data.vertex_buffer, "VertexBuffer is nullptr.");
    JOJ_ASSERT(stride, "stride is nullptr.");
    JOJ_ASSERT(offset, "offset is nullptr.");

    m_cmd_list->device_context->IASetVertexBuffers(start_slot, num_buffers, &m_data.vertex_buffer, stride, offset);
}

void joj::D3D11VertexBuffer::unbind()
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    m_cmd_list->device_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}

b8 joj::D3D11VertexBuffer::is_filled() const
{
    return m_filled;
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

joj::ErrorCode joj::D3D11VertexBuffer::update_internal(const void* data, const u32 sizeof_data)
{
    JOJ_ASSERT(data, "data is nullptr");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr");

    if (m_data.vbd.Usage != D3D11_USAGE_DYNAMIC)
    {
        JOJ_ERROR(ErrorCode::ERR_VERTEX_BUFFER_UPDATE,
            "Vertex Buffer usage is not dynamic.");
        return ErrorCode::ERR_VERTEX_BUFFER_UPDATE;
    }

    if (m_data.vbd.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE)
    {
        JOJ_ERROR(ErrorCode::ERR_VERTEX_BUFFER_UPDATE,
            "Vertex Buffer CPUAcessFlags is not writable.");
        return ErrorCode::ERR_VERTEX_BUFFER_UPDATE;
    }

    // Data to be copied
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
    mapped_buffer.pData = nullptr;

    // Get a pointer to the vertex buffer data and lock it
    if (m_cmd_list->device_context->Map(m_data.vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_VERTEX_MAP_FAILED,
            "Failed to map vertex buffer.");
        return ErrorCode::ERR_VERTEX_MAP_FAILED;
    }
    else
    {
        // Copy the new data to the constant buffer data.
        memcpy(mapped_buffer.pData, data, sizeof_data);

        // Release the pointer to the constant buffer data.
        m_cmd_list->device_context->Unmap(m_data.vertex_buffer, 0);
    }

    return ErrorCode::OK;
}

#endif // JOJ_PLATFORM_WINDOWS