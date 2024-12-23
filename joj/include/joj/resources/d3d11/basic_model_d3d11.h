#ifndef _JOJ_D3D11_BASIC_MODEL_H
#define _JOJ_D3D11_BASIC_MODEL_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "resources/basic_model.h"

namespace joj
{
    class JAPI D3D11BasicModel : public BasicModel
    {
    public:
        D3D11BasicModel();
        ~D3D11BasicModel();

        ErrorCode load_m3d(GraphicsDevice& device, CommandList& cmd_list,
            TextureManager& tex_mgr, const std::string& model_filename,
            const std::wstring& texture_path) override;
    };
}

#endif // JPLATFORM_WINDOWS

#endif _JOJ_D3D11_BASIC_MODEL_H