#ifndef _JOJ_RENDERER_H
#define _JOJ_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"

JAPI void renderer_print();

namespace joj
{
    struct RendererData;

    class JAPI IRenderer
    {
    public:
        IRenderer();
        virtual ~IRenderer();

        virtual ErrorCode create_context() = 0;
        virtual void destroy_context() = 0;

        virtual ErrorCode initialize() = 0;
        virtual void shutdown() = 0;

        virtual void resize(i32 width, i32 height) = 0;

        virtual RendererData& get_data() = 0;
    };
}

#endif // _JOJ_RENDERER_H