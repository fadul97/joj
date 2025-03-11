#ifndef _JOJ_D3D11_MODEL_MANAGER_H
#define _JOJ_D3D11_MODEL_MANAGER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/resources/model_manager.h"
#include <unordered_map>
#include <string>
#include "basic_model_d3d11.h"
#include "joj/renderer/d3d11/texture_manager_d3d11.h"

namespace joj
{
    class JOJ_API D3D11ModelManager : public ModelManager<D3D11BasicModel>
    {
    public:
        D3D11ModelManager();
        ~D3D11ModelManager();

        ErrorCode load_model(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_path, const std::wstring& texture_path) override;
        void destroy() override;

        D3D11BasicModel* get_model(const std::string& model_path) override;

    private:
        std::map<std::string, D3D11BasicModel*> m_models;
        D3D11TextureManager m_tex_mgr;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_MODEL_MANAGER_H