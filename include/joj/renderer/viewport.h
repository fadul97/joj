#ifndef _JOJ_VIEWPORT_H
#define _JOJ_VIEWPORT_H

#include "joj/core/defines.h"

namespace joj
{
    class JOJ_API Viewport
    {
    public:
        Viewport();
        Viewport(f32 x, f32 y, f32 width, f32 height, f32 min_depth, f32 max_depth);
        ~Viewport();

        void set(f32 x, f32 y, f32 width, f32 height, f32 min_depth, f32 max_depth);
        void set_size(const u32 width, const u32 height);

    // FIXME: Make these private or rename them
    public:
        f32 m_x = 0.0f;
        f32 m_y = 0.0f;
        f32 m_width = 0.0f;
        f32 m_height = 0.0f;
        f32 m_min_depth = 0.0f;
        f32 m_max_depth = 0.0f;
    };
}

#endif // _JOJ_VIEWPORT_H