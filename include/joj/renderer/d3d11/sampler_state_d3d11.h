#ifndef _JOJ_D3D11_SAMPLER_STATE_H
#define _JOJ_D3D11_SAMPLER_STATE_H

#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include <d3d11.h>
#include "renderer/sampler_state.h"
#include <renderer/sampler_state.h>

namespace joj
{
    struct SamplerData
    {
        ID3D11SamplerState* sampler_state;
    };

    class JAPI D3D11SamplerState : public SamplerState
    {
    public:
        D3D11SamplerState();
        ~D3D11SamplerState();

        ErrorCode create(GraphicsDevice& device, SamplerDesc& desc) override;

        void bind(CommandList& cmd_list, SamplerType type,
            u32 start_slot, u32 num_samplers) override;

        SamplerData& get_data() override;

    private:
        SamplerData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_SAMPLER_STATE_H