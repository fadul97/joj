#ifndef _JOJ_TEXTURE_2D_H
#define _JOJ_TEXTURE_2D_H

#include "core/defines.h"

#include "core/error_code.h"
#include <string>
#include "renderer.h"
#include "image_type.h"
#include "texture2d_data.h"

namespace joj
{
    class JOJ_API Texture2D
    {
    public:
        Texture2D();
        virtual ~Texture2D();

        virtual ErrorCode create(GraphicsDevice& device, CommandList& cmd_list,
            const std::wstring& filepath, ImageType type) = 0;
        virtual void destroy() = 0;

        virtual void bind(CommandList& cmd_list, u32 start_slot, u32 num_views) = 0;
        virtual void unbind(CommandList& cmd_list) = 0;

        virtual TextureData2D& get_data() = 0;

        u32 get_width() const;
        u32 get_height() const;
    protected:
        u32 m_width;
        u32 m_height;
    };

    inline u32 Texture2D::get_width() const
    { return m_width; }

    inline u32 Texture2D::get_height() const
    { return m_height; }
}

#endif // _JOJ_TEXTURE_H