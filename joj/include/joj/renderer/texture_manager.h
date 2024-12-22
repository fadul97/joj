#ifndef _JOJ_TEXTURE_MANAGER_H
#define _JOJ_TEXTURE_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <map>

#include "error_code.h"
#include <string>
#include "renderer.h"
#include "image_type.h"
#include "texture2d_data.h"

namespace joj
{
    class JAPI TextureManager
    {
    public:
        TextureManager();
        virtual ~TextureManager();

        virtual ErrorCode create(GraphicsDevice& device, CommandList& cmd_list,
            const std::wstring& filename, ImageType type) = 0;
        virtual void destroy() = 0;

        /*
    protected:
        std::map<std::wstring, Texture2DData> m_texture_SRV;
        */
    };
}

#endif // _JOJ_TEXTURE_MANAGER_H