#ifndef _JOJ_D3D11_INPUT_LAYOUT_H
#define _JOJ_D3D11_INPUT_LAYOUT_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/input_layout.h"
#include <d3d11.h>

namespace joj
{
    struct InputLayoutData
    {
        ID3D11InputLayout* input_layout;
    };

    class JOJ_API D3D11InputLayout : public InputLayout
    {
    public:
        D3D11InputLayout();
        ~D3D11InputLayout();

        ErrorCode create(const GraphicsDevice& device, VertexShader& shader) override;

        void bind(CommandList& cmd_list) override;

        InputLayoutData& get_data() override;

    private:
        InputLayoutData m_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_INPUT_LAYOUT_H