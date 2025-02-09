#include "renderer/d3d11/input_layout_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "renderer/d3d11/shader_d3d11.h"
#include "core/logger.h"

joj::D3D11InputLayout::D3D11InputLayout()
{
    m_data.input_layout = nullptr;
}

joj::D3D11InputLayout::~D3D11InputLayout()
{
    // Release resource
    if (m_data.input_layout)
    {
        m_data.input_layout->Release();
        m_data.input_layout = nullptr;
    }
}

joj::ErrorCode joj::D3D11InputLayout::create(const GraphicsDevice& device,
    VertexShader& shader)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> input_desc;
    for (const InputDesc& desc : m_input_desc)
    {
        D3D11_INPUT_ELEMENT_DESC d3d11_desc = { 0 };
        d3d11_desc.SemanticName = desc.semantic_name.c_str();
        d3d11_desc.SemanticIndex = desc.semantic_index;
        
        switch (desc.format)
        {
        case DataFormat::R32G32B32A32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        case DataFormat::R32G32B32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case DataFormat::R32G32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
            break;
        case DataFormat::R32_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R32_UINT;
            break;
        case DataFormat::R32G32B32A32_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            break;
        case DataFormat::R8G8B8A8_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
        default:
            break;
        }

        d3d11_desc.InputSlot = desc.input_slot;
        d3d11_desc.AlignedByteOffset = desc.aligned_byte_offset;

        switch (desc.classification)
        {
        case InputClassification::PerVertexData:
            d3d11_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            break;
        case InputClassification::PerInstanceData:
            d3d11_desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            break;
        default:
            break;
        }

        d3d11_desc.InstanceDataStepRate = desc.instance_data_step_rate;

        input_desc.push_back(d3d11_desc);
    }

    if (device.device->CreateInputLayout(input_desc.data(), input_desc.size(),
        shader.vsblob->GetBufferPointer(),
        shader.vsblob->GetBufferSize(), &m_data.input_layout) != S_OK)
    {
        JERROR(ErrorCode::ERR_INPUT_LAYOUT_D3D11_CREATION,
            "Failed to create D3D11 Input Layout");
        return ErrorCode::ERR_INPUT_LAYOUT_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11InputLayout::bind(CommandList& cmd_list)
{
    cmd_list.device_context->IASetInputLayout(m_data.input_layout);
}

joj::InputLayoutData& joj::D3D11InputLayout::get_data()
{
    return m_data;
}

#endif // JPLATFORM_WINDOWS