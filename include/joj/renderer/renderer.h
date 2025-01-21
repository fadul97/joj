#ifndef _JOJ_RENDERER_H
#define _JOJ_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include "platform/window.h"
#include "viewport.h"
#include "sprite.h"

JAPI void renderer_print();

namespace joj
{
    enum class PrimitiveTopology
    {
        UNDEFINED,
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };

    enum class RasterizerState
    {
        Wireframe,
        Solid
    };

    struct GraphicsDevice;

    struct CommandList;

    class JAPI IRenderer
    {
    public:
        IRenderer();
        virtual ~IRenderer();

        virtual ErrorCode create_context() = 0;
        virtual void destroy_context() = 0;

        virtual ErrorCode initialize(WindowData window) = 0;
        virtual void shutdown() = 0;

        virtual void enable_depth_test() = 0;
        virtual void disable_depth_test() = 0;

        virtual void set_rasterizer_state(const RasterizerState state) = 0;
        virtual void set_primitive_topology(const PrimitiveTopology topology) = 0;

        virtual void set_viewport(const f32 x, const f32 y, const f32 width,
            const f32 height, const f32 min_depth, const f32 max_depth) = 0;
        virtual void set_viewport(const Viewport& viewport) = 0;
        virtual void set_viewport_size(const u32 width, const u32 height) = 0;

        virtual void resize(i32 width, i32 height) = 0;

        virtual GraphicsDevice& get_device() = 0;
        virtual CommandList& get_cmd_list() = 0;

        virtual void clear(f32 r = 0.23f, f32 g = 0.23f, f32 b = 0.23f, f32 a = 1.0f) = 0;
        virtual void swap_buffers() = 0;

        virtual void draw_sprite(const SpriteData& sprite) = 0;
    };
}

#endif // _JOJ_RENDERER_H