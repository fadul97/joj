#ifndef JOJ_GL_RENDERER_H
#define JOJ_GL_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "renderer/renderer.h"

namespace joj
{
    class JAPI GLRenderer : public Renderer
    {
    public:
        GLRenderer();
        ~GLRenderer();
        
        b8 init(std::unique_ptr<Window>& window) override;
        void render() override;
        void clear(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 0.0f) override;
        void shutdown() override;
    };
}


#endif // JPLATFORM_LINUX

#endif // JOJ_GL_RENDERER_H