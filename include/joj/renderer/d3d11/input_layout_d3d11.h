#ifndef _JOJ_D3D11_INPUT_LAYOUT_H
#define _JOJ_D3D11_INPUT_LAYOUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/input_layout.h"
#include <d3d11.h>

namespace joj
{
    struct InputLayoutData
    {
        ID3D11InputLayout* input_layout;
    };

    class JAPI D3D11InputLayout : public InputLayout
    {
    public:
        D3D11InputLayout();
        ~D3D11InputLayout();

        ErrorCode create(GraphicsDevice& device, VertexShader& shader) override;

        void bind(CommandList& cmd_list) override;

        InputLayoutData& get_data() override;

    private:
        InputLayoutData m_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_INPUT_LAYOUT_H