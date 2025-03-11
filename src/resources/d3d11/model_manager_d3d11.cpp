#include "resources/d3d11/model_manager_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "core/jmacros.h"

joj::D3D11ModelManager::D3D11ModelManager()
{
    m_tex_mgr = D3D11TextureManager();
}

joj::D3D11ModelManager::~D3D11ModelManager()
{
}

joj::ErrorCode joj::D3D11ModelManager::load_model(GraphicsDevice& device, CommandList& cmd_list,
    const std::string& model_path, const std::wstring& texture_path)
{
    joj::D3D11BasicModel* model = new joj::D3D11BasicModel();
    if JOJ_FAILED(model->load_m3d(device, cmd_list, m_tex_mgr, model_path, texture_path))
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED,
            "Failed to load model '%ls'.", model_path.c_str());
        return ErrorCode::ERR_FILE_LOAD_M3D_STATIC_FAILED;
    }

    m_models[model_path] = model;
    return ErrorCode::OK;
}

void joj::D3D11ModelManager::destroy()
{
    m_models.clear();
}

joj::D3D11BasicModel* joj::D3D11ModelManager::get_model(const std::string& model_path)
{
    if (m_models.find(model_path) != m_models.end())
    {
        return m_models[model_path];
    }

    return nullptr;
}

#endif // JOJ_PLATFORM_WINDOWS