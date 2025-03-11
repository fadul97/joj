#ifndef _JOJ_D3D11_BASIC_MODEL_H
#define _JOJ_D3D11_BASIC_MODEL_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "resources/basic_model.h"

namespace joj
{
    class JOJ_API D3D11BasicModel : public BasicModel
    {
    public:
        D3D11BasicModel();
        ~D3D11BasicModel();

        ErrorCode load_obj(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_filename) override;

        ErrorCode load_m3d(GraphicsDevice& device, CommandList& cmd_list,
            TextureManager& tex_mgr, const std::string& model_filename,
            const std::wstring& texture_path) override;

        ErrorCode load_jsf(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_filename) override;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_BASIC_MODEL_H