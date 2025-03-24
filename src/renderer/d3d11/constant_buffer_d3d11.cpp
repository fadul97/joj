#include "joj/renderer/d3d11/constant_buffer_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/jmacros.h"
#include "joj/renderer/d3d11/graphics_device_d3d11.h"
#include "joj/renderer/d3d11/command_list_d3d11.h"
#include "joj/core/logger.h"

u32 joj::calculate_cb_byte_size(const u32 byte_size)
{
    // Constant buffers must be a multiple of the minimum hardware
    // allocation size (usually 256 bytes).  So round up to nearest
    // multiple of 256.  We do this by adding 255 and then masking off
    // the lower 2 bytes which store all bits < 256.
    // Example: Suppose byte_size = 300.
    // (300 + 255) & ~255
    // 555 & ~255
    // 0x022B & ~0x00ff
    // 0x022B & 0xff00
    // 0x0200
    // 512
    return (byte_size + 255) & ~255;
}

joj::D3D11ConstantBuffer::D3D11ConstantBuffer()
    : m_device(nullptr), m_cmd_list(nullptr), m_filled(false)
{
    m_data.cbd = { 0 };
    m_data.constant_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11ConstantBuffer::D3D11ConstantBuffer(GraphicsDevice* device, CommandList* cmd_list)
    : m_device(device), m_cmd_list(cmd_list), m_filled(false)
{
    m_data.cbd = { 0 };
    m_data.constant_buffer = nullptr;
    m_data.init_data = { 0 };
    m_data.init_data.pSysMem = nullptr;
}

joj::D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
    cleanup();
}

joj::ErrorCode joj::D3D11ConstantBuffer::create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
    const u32 byte_width, const void* data)
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    // Describe Buffer - Resource structure
    m_data.cbd.ByteWidth = byte_width;
    m_data.cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    switch (usage)
    {
    case BufferUsage::Default:
        m_data.cbd.Usage = D3D11_USAGE_DEFAULT;
        break;
    case BufferUsage::Immutable:
        m_data.cbd.Usage = D3D11_USAGE_IMMUTABLE;
        break;
    case BufferUsage::Dynamic:
        m_data.cbd.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case BufferUsage::Staging:
        m_data.cbd.Usage = D3D11_USAGE_STAGING;
        break;
    default:
        m_data.cbd.Usage = D3D11_USAGE_DEFAULT;
        break;
    }

    switch (cpu_access_flags)
    {
    case CPUAccessType::None:
        m_data.cbd.CPUAccessFlags = 0;
        break;
    case CPUAccessType::Write:
        m_data.cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    case CPUAccessType::Read:
        m_data.cbd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
    default:
        break;
    }

    m_data.cbd.MiscFlags = 0;
    m_data.cbd.StructureByteStride = 0;

    if (data != nullptr)
        m_data.init_data.pSysMem = data;

    if (m_device->device->CreateBuffer(
        &m_data.cbd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.constant_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_CONSTANT_BUFFER_CREATION,
            "Failed to create D3D11 Constant Buffer.");
        return ErrorCode::ERR_CONSTANT_BUFFER_CREATION;
    }

    m_filled = true;

    return ErrorCode::OK;
}

void joj::D3D11ConstantBuffer::bind_to_vertex_shader(const u32 start_slot, const u32 num_buffers)
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");
    JOJ_ASSERT(m_data.constant_buffer, "ConstantBuffer is nullptr.");

    m_cmd_list->device_context->VSSetConstantBuffers(start_slot, num_buffers, &m_data.constant_buffer);
}

void joj::D3D11ConstantBuffer::bind_to_pixel_shader(const u32 start_slot, const u32 num_buffers)
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");
    JOJ_ASSERT(m_data.constant_buffer, "ConstantBuffer is nullptr.");

    m_cmd_list->device_context->PSSetConstantBuffers(start_slot, num_buffers, &m_data.constant_buffer);
}

void joj::D3D11ConstantBuffer::unbind_from_vertex_shader()
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    ID3D11Buffer* buffer = nullptr;
    m_cmd_list->device_context->VSSetConstantBuffers(0, 1, &buffer);
}

void joj::D3D11ConstantBuffer::unbind_from_pixel_shader()
{
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    ID3D11Buffer* buffer = nullptr;
    m_cmd_list->device_context->PSSetConstantBuffers(0, 1, &buffer);
}

b8 joj::D3D11ConstantBuffer::is_filled() const
{
    return m_filled;
}

void joj::D3D11ConstantBuffer::cleanup()
{
    if (m_filled)
    {
        // Release Constant Buffer
        if (m_data.constant_buffer)
        {
            m_data.constant_buffer->Release();
            m_data.constant_buffer = nullptr;
        }

        m_data.init_data.pSysMem = nullptr;
    }

    m_filled = false;
}

joj::ErrorCode joj::D3D11ConstantBuffer::update_internal(const void* data, const u32 sizeof_data)
{
    JOJ_ASSERT(data, "data is nullptr");
    JOJ_ASSERT(m_cmd_list, "CommandList is nullptr");

    if (m_data.cbd.Usage != D3D11_USAGE_DYNAMIC)
    {
        JOJ_ERROR(ErrorCode::ERR_CONSTANT_BUFFER_UPDATE,
            "Constant Buffer usage is not dynamic.");
        return ErrorCode::ERR_CONSTANT_BUFFER_UPDATE;
    }

    if (m_data.cbd.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE)
    {
        JOJ_ERROR(ErrorCode::ERR_CONSTANT_BUFFER_UPDATE,
            "Constant Buffer CPUAcessFlags is not writable.");
        return ErrorCode::ERR_CONSTANT_BUFFER_UPDATE;
    }

    // Data to be copied
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
    mapped_buffer.pData = nullptr;

    // Get a pointer to the constant buffer data and lock it
    if (m_cmd_list->device_context->Map(m_data.constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_CONSTANT_MAP_FAILED,
            "Failed to map constant buffer.");
        return ErrorCode::ERR_CONSTANT_MAP_FAILED;
    }
    else
    {
        // Copy the new data to the constant buffer data.
        memcpy(mapped_buffer.pData, data, sizeof_data);

        // Release the pointer to the constant buffer data.
        m_cmd_list->device_context->Unmap(m_data.constant_buffer, 0);
    }

    return ErrorCode::OK;
}

#endif // JOJ_PLATFORM_WINDOWS