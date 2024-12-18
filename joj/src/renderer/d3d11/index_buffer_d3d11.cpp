#include "renderer/d3d11/index_buffer_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "logger.h"

joj::D3D11IndexBuffer::D3D11IndexBuffer()
{
    m_data.ibd = { 0 };
    m_data.index_buffer = nullptr;
    m_data.init_data = { 0 };
}

joj::D3D11IndexBuffer::~D3D11IndexBuffer()
{
    // Release Index Buffer
    if (m_data.index_buffer)
    {
        m_data.index_buffer->Release();
        m_data.index_buffer = nullptr;
    }
}

void joj::D3D11IndexBuffer::setup(u32 byte_width, const void* data)
{
    // Describe Index Buffer
    m_data.ibd.Usage = D3D11_USAGE_IMMUTABLE;
    m_data.ibd.ByteWidth = byte_width;
    m_data.ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_data.ibd.CPUAccessFlags = 0;
    m_data.ibd.MiscFlags = 0;
    m_data.ibd.StructureByteStride = 0;

    if (data != nullptr)
        m_data.init_data.pSysMem = data;
}

joj::ErrorCode joj::D3D11IndexBuffer::create(GraphicsDevice& device)
{
    if (device.device->CreateBuffer(
        &m_data.ibd,
        (m_data.init_data.pSysMem != nullptr ? &m_data.init_data : nullptr),
        &m_data.index_buffer) != S_OK)
    {
        JERROR(ErrorCode::ERR_INDEX_BUFFER_D3D11_CREATION,
            "Failed to create D3D11 Index Buffer.");
        return ErrorCode::ERR_VERTEX_BUFFER_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11IndexBuffer::bind(CommandList& cmd_list, DataFormat format,
    const u32 offset)
{
    switch (format)
    {
    case DataFormat::R32_UINT:
        cmd_list.device_context->IASetIndexBuffer(m_data.index_buffer,
            DXGI_FORMAT_R32_UINT, offset);
        break;
    default:
        break;
    }
}

joj::IBData& joj::D3D11IndexBuffer::get_data()
{
    return m_data;
}

#endif // JPLATFORM_WINDOWS