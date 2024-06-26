#ifndef JOJ_RENDERER_H
#define JOJ_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"

namespace joj
{
    template<class Twindow>
    class JAPI IRenderer
    {
    public:
        IRenderer();
        virtual ~IRenderer() = 0;

        virtual ErrorCode init(Twindow& window) = 0;
        virtual void render() = 0;
        virtual void clear() = 0;
        virtual void clear(f32 r, f32 g, f32 b, f32 a) = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;

        virtual ErrorCode setup_default_pipeline(Twindow& window) = 0;
    };
}

#endif // JOJ_RENDERER_H
