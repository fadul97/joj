#include "renderer/d3d11/sampler_state_d3d11.h"

#if JPLATFORM_WINDOWS

#include "renderer/d3d11/renderer_d3d11.h"
#include "logger.h"

joj::D3D11SamplerState::D3D11SamplerState()
{
    m_data.sampler_state = nullptr;
}

joj::D3D11SamplerState::~D3D11SamplerState()
{
    // Release Sampler State
    if (m_data.sampler_state)
    {
        m_data.sampler_state->Release();
        m_data.sampler_state = nullptr;
    }
}

joj::ErrorCode joj::D3D11SamplerState::create(GraphicsDevice& device,
    SamplerDesc& desc)
{
    if (m_data.sampler_state != nullptr)
        return ErrorCode::OK;

    D3D11_SAMPLER_DESC sampler_desc = {};
    sampler_desc.MaxAnisotropy = desc.max_anisotropy;
    sampler_desc.MipLODBias = desc.mip_lod_bias;
    sampler_desc.BorderColor[0] = desc.border_color[0];
    sampler_desc.BorderColor[1] = desc.border_color[1];
    sampler_desc.BorderColor[2] = desc.border_color[2];
    sampler_desc.BorderColor[3] = desc.border_color[3];

    switch (desc.filter)
    {
    case SamplerFilter::MIN_MAG_MIP_POINT:
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        break;
    case SamplerFilter::MIN_MAG_POINT_MIP_LINEAR:
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        break;
    case SamplerFilter::MIN_MAG_MIP_LINEAR:
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        break;
    case SamplerFilter::ANISOTROPIC:
        sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
        break;
    default:
        break;
    }

    switch (desc.addressU)
    {
    case TextureAddressMode::Wrap:
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        break;
    case TextureAddressMode::Mirror:
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
        break;
    case TextureAddressMode::Clamp:
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        break;
    case TextureAddressMode::Border:
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        break;
    case TextureAddressMode::MirrorOnce:
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        break;
    default:
        break;
    }

    switch (desc.addressV)
    {
    case TextureAddressMode::Wrap:
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        break;
    case TextureAddressMode::Mirror:
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
        break;
    case TextureAddressMode::Clamp:
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        break;
    case TextureAddressMode::Border:
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        break;
    case TextureAddressMode::MirrorOnce:
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        break;
    default:
        break;
    }

    switch (desc.addressW)
    {
    case TextureAddressMode::Wrap:
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        break;
    case TextureAddressMode::Mirror:
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
        break;
    case TextureAddressMode::Clamp:
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        break;
    case TextureAddressMode::Border:
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        break;
    case TextureAddressMode::MirrorOnce:
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        break;
    default:
        break;
    }

    switch (desc.func)
    {
    case ComparisonFunc::Never:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        break;
    case ComparisonFunc::Less:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
        break;
    case ComparisonFunc::Equal:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
        break;
    case ComparisonFunc::LessEqual:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        break;
    case ComparisonFunc::Greater:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_GREATER;
        break;
    case ComparisonFunc::NotEqual:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
        break;
    case ComparisonFunc::GreaterEqual:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
        break;
    case ComparisonFunc::Always:
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        break;
    default:
        break;
    }

    switch (desc.min_LOD)
    {
    case LODValue::Zero:
        sampler_desc.MinLOD = 0.0f;
        break;
    case LODValue::Float32_MAX:
        sampler_desc.MinLOD = D3D11_FLOAT32_MAX;
        break;
    default:
        break;
    }

    switch (desc.max_LOD)
    {
    case LODValue::Zero:
        sampler_desc.MaxLOD = 0.0f;
        break;
    case LODValue::Float32_MAX:
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
        break;
    default:
        break;
    }

    if (device.device->CreateSamplerState(&sampler_desc, &m_data.sampler_state) != S_OK)
    {
        JERROR(ErrorCode::ERR_SAMPLER_STATE_D3D11_CREATION, "Failed to create Sampler State.");
        return ErrorCode::ERR_SAMPLER_STATE_D3D11_CREATION;
    }

    return ErrorCode::OK;
}

void joj::D3D11SamplerState::bind(CommandList& cmd_list, SamplerType type,
    u32 start_slot, u32 num_samplers)
{
    switch (type)
    {
    case joj::SamplerType::Anisotropic:
        cmd_list.device_context->PSSetSamplers(start_slot, num_samplers,
            &m_data.sampler_state);
        break;
    case joj::SamplerType::Pixelated:
        cmd_list.device_context->PSSetSamplers(start_slot, num_samplers,
            &m_data.sampler_state);
        break;
    default:
        break;
    }
}

joj::SamplerData& joj::D3D11SamplerState::get_data()
{
    return m_data;
}

#endif // JPLATFORM_WINDOWS