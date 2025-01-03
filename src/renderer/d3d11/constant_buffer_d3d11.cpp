#include "renderer/d3d11/constant_buffer_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "logger.h"

joj::D3D11ConstantBuffer::D3D11ConstantBuffer()
{
    m_data.cbd = { 0 };
    m_data.buffer = nullptr;
    m_data.init_data = { 0 };
}


joj::D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
    if (m_data.buffer != nullptr)
    {
        m_data.buffer->Release();
        m_data.buffer = nullptr;
    }
}

void joj::D3D11ConstantBuffer::setup(const u32 byte_width, const void* data)
{
    m_data.cbd.ByteWidth = byte_width;
    m_data.cbd.Usage = D3D11_USAGE_DYNAMIC;
    m_data.cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_data.cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (data != nullptr)
        m_data.init_data.pSysMem = data;
}

joj::ErrorCode joj::D3D11ConstantBuffer::create(GraphicsDevice& device)
{
    if (device.device->CreateBuffer(
        &m_data.cbd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.buffer) != S_OK)
    {
        JERROR(ErrorCode::ERR_CONSTANT_BUFFER_D3D11_CREATION,
            "Failed to create D3D11 Constant Buffer.");
        return ErrorCode::ERR_CONSTANT_BUFFER_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11ConstantBuffer::bind_to_vertex_shader(CommandList& cmd_list,
    u32 start_slot, u32 num_buffers)
{
    cmd_list.device_context->VSSetConstantBuffers(start_slot, num_buffers, &m_data.buffer);
}

void joj::D3D11ConstantBuffer::bind_to_pixel_shader(CommandList& cmd_list,
    u32 start_slot, u32 num_buffers)
{
    cmd_list.device_context->PSSetConstantBuffers(start_slot, num_buffers, &m_data.buffer);
}

joj::CBData& joj::D3D11ConstantBuffer::get_data()
{
    return m_data;
}

void joj::D3D11ConstantBuffer::update_internal(CommandList& cmd_list, const void* data, const u32 sizeof_data)
{
    // Data to be copied
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};

    // Get a pointer to the constant buffer data and lock it
    cmd_list.device_context->Map(m_data.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

    // Copy the new data to the constant buffer data.
    memcpy(mapped_buffer.pData, data, sizeof_data);

    // Release the pointer to the constant buffer data.
    cmd_list.device_context->Unmap(m_data.buffer, 0);
}

#endif // JPLATFORM_WINDOWS