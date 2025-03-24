#include "joj/renderer/d3d11/index_buffer_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/jmacros.h"
#include "joj/renderer/d3d11/graphics_device_d3d11.h"
#include "joj/renderer/d3d11/command_list_d3d11.h"
#include "joj/core/logger.h"

joj::D3D11IndexBuffer::D3D11IndexBuffer()
    : m_device(nullptr), m_cmd_list(nullptr), m_filled(false)
{
    m_data.ibd = { 0 };
    m_data.index_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11IndexBuffer::D3D11IndexBuffer(GraphicsDevice* device, CommandList* cmd_list)
    : m_device(device), m_cmd_list(cmd_list), m_filled(false)
{
    m_data.ibd = { 0 };
    m_data.index_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11IndexBuffer::~D3D11IndexBuffer()
{
    cleanup();
}

joj::ErrorCode joj::D3D11IndexBuffer::create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
    const u32 byte_width, const void* data)
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    // Describe Buffer - Resource structure
    m_data.ibd.ByteWidth = byte_width;
    m_data.ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    switch (usage)
    {
    case BufferUsage::Default:
        m_data.ibd.Usage = D3D11_USAGE_DEFAULT;
        break;
    case BufferUsage::Immutable:
        m_data.ibd.Usage = D3D11_USAGE_IMMUTABLE;
        break;
    case BufferUsage::Dynamic:
        m_data.ibd.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case BufferUsage::Staging:
        m_data.ibd.Usage = D3D11_USAGE_STAGING;
        break;
    default:
        m_data.ibd.Usage = D3D11_USAGE_DEFAULT;
        break;
    }

    switch (cpu_access_flags)
    {
    case CPUAccessType::None:
        m_data.ibd.CPUAccessFlags = 0;
        break;
    case CPUAccessType::Write:
        m_data.ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    case CPUAccessType::Read:
        m_data.ibd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
    default:
        break;
    }

    m_data.ibd.MiscFlags = 0;
    m_data.ibd.StructureByteStride = 0;

    if (data != nullptr)
        m_data.init_data.pSysMem = data;

    if (m_device->device->CreateBuffer(
        &m_data.ibd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.index_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_INDEX_BUFFER_CREATION,
            "Failed to create D3D11 Index Buffer.");
        return ErrorCode::ERR_INDEX_BUFFER_CREATION;
    }

    m_filled = true;

    return ErrorCode::OK;
}

void joj::D3D11IndexBuffer::bind(const DataFormat format, const u32 offset)
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");
    JOJ_ASSERT(m_data.index_buffer, "IndexBuffer is nullptr.");

    switch (format)
    {
    case DataFormat::R32_UINT:
        m_cmd_list->device_context->IASetIndexBuffer(m_data.index_buffer,
            DXGI_FORMAT_R32_UINT, offset);
        break;
    case DataFormat::R16_UINT:
        m_cmd_list->device_context->IASetIndexBuffer(m_data.index_buffer,
            DXGI_FORMAT_R16_UINT, offset);
        break;
    default:
        break;
    }
}

void joj::D3D11IndexBuffer::unbind()
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    m_cmd_list->device_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

b8 joj::D3D11IndexBuffer::is_filled() const
{
    return m_filled;
}

void joj::D3D11IndexBuffer::cleanup()
{
    if (m_filled)
    {
        // Release Index Buffer
        if (m_data.index_buffer)
        {
            m_data.index_buffer->Release();
            m_data.index_buffer = nullptr;
        }

        m_data.init_data.pSysMem = nullptr;
    }

    m_filled = false;
}

joj::ErrorCode joj::D3D11IndexBuffer::update_internal(const void* data, const u32 sizeof_data)
{
    JOJ_ASSERT(data, "data is nullptr");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr");

    if (m_data.ibd.Usage != D3D11_USAGE_DYNAMIC)
    {
        JOJ_ERROR(ErrorCode::ERR_INDEX_BUFFER_UPDATE,
            "Index Buffer usage is not dynamic.");
        return ErrorCode::ERR_INDEX_BUFFER_UPDATE;
    }

    if (m_data.ibd.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE)
    {
        JOJ_ERROR(ErrorCode::ERR_INDEX_BUFFER_UPDATE,
            "Index Buffer CPUAcessFlags is not writable.");
        return ErrorCode::ERR_INDEX_BUFFER_UPDATE;
    }

    // Data to be copied
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
    mapped_buffer.pData = nullptr;

    // Get a pointer to the index buffer data and lock it
    if (m_cmd_list->device_context->Map(m_data.index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_INDEX_MAP_FAILED,
            "Failed to map index buffer.");
        return ErrorCode::ERR_INDEX_MAP_FAILED;
    }
    else
    {
        // Copy the new data to the constant buffer data.
        memcpy(mapped_buffer.pData, data, sizeof_data);

        // Release the pointer to the constant buffer data.
        m_cmd_list->device_context->Unmap(m_data.index_buffer, 0);
    }

    return ErrorCode::OK;
}

#endif // JOJ_PLATFORM_WINDOWS