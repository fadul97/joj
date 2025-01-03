#ifndef _JOJ_MODEL_MANAGER_H
#define _JOJ_MODEL_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "basic_model.h"

namespace joj
{
    template<typename T>
    class JAPI ModelManager
    {
    public:
        ModelManager();
        virtual ~ModelManager();

        virtual ErrorCode load_model(GraphicsDevice& device, CommandList& cmd_list,
            const std::string& model_path, const std::wstring& texture_path) = 0;
        virtual void destroy() = 0;

        virtual T* get_model(const std::string& model_path) = 0;

/*
    protected:
        std::map<std::wstring, BasicModel> m_models;
*/
    };

    template<typename T>
    ModelManager<T>::ModelManager()
    {
    }

    template<typename T>
    ModelManager<T>::~ModelManager()
    {
    }
}

#endif // _JOJ_MODEL_MANAGER_H