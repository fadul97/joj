#ifndef _JOJ_D3D11_BASIC_SKINNED_MODEL_H
#define _JOJ_D3D11_BASIC_SKINNED_MODEL_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "resources/basic_skinned_model.h"

namespace joj
{
    class JAPI D3D11BasicSkinnedModel : public BasicSkinnedModel
    {
    public:
        D3D11BasicSkinnedModel();
        ~D3D11BasicSkinnedModel();

        ErrorCode load_m3d(GraphicsDevice& device, CommandList& cmd_list,
            TextureManager& tex_mgr, const std::string& model_filename,
            const std::wstring& texture_path) override;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_BASIC_SKINNED_MODEL_H