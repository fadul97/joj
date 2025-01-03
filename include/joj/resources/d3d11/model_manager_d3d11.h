#ifndef _JOJ_D3D11_MODEL_MANAGER_H
#define _JOJ_D3D11_MODEL_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "resources/model_manager.h"
#include <unordered_map>
#include <string>
#include "basic_model_d3d11.h"
#include "renderer/d3d11/texture_manager_d3d11.h"

namespace joj
{
    class JAPI D3D11ModelManager : public ModelManager<D3D11BasicModel>
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

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_MODEL_MANAGER_H