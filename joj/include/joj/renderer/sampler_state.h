#ifndef _JOJ_SAMPLER_STATE_H
#define _JOJ_SAMPLER_STATE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "renderer.h"

namespace joj
{
    enum class SamplerType
    {
        Anisotropic,
        Pixelated,
    };

    enum class SamplerFilter
    {
        MIN_MAG_MIP_POINT,
        MIN_MAG_POINT_MIP_LINEAR,
        MIN_MAG_MIP_LINEAR,
        Anisotropic,
    };

    enum class TextureAddressMode
    {
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce
    };

    enum class ComparisonFunc
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    enum class LodValue
    {
        Zero,
        Float32_MAX,
    };

    struct SamplerDesc
    {
        SamplerFilter filter;
        TextureAddressMode addressU;
        TextureAddressMode addressV;
        TextureAddressMode addressW;
        f32 mip_lod_bias;
        u32 max_anisotropy;
        ComparisonFunc func;
        f32 border_color[4];
        LodValue min_lod;
        LodValue max_lod;

        /*
         * TODO: Default values? 
        SamplerFilter filter = SamplerFilter::MIN_MAG_MIP_LINEAR;
        TextureAddressMode addressU = TextureAddressMode::Wrap;
        TextureAddressMode addressV = TextureAddressMode::Wrap;
        TextureAddressMode addressW = TextureAddressMode::Wrap;
        f32 mip_lod_bias = 0.0f;
        u32 max_anisotropy = 0;
        ComparisonFunc func;
        f32 border_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        f32 min_lod = 0.0f;
        f32 max_lod = 0.0f;
         */
    };

    struct SamplerData;

    class JAPI SamplerState
    {
    public:
        SamplerState();
        virtual ~SamplerState();

        virtual ErrorCode create(SamplerDesc& desc) = 0;

        virtual void bind(CommandList& cmd_list, SamplerType type,
            u32 start_slot, u32 num_samplers) = 0;

        virtual SamplerData& get_data() const = 0;
    };
}

#endif // _JOJ_SAMPLER_STATE_H