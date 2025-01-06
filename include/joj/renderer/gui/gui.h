#ifndef _JOJ_GUI_H
#define _JOJ_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"

namespace joj
{
    class JAPI IGUI
    {
    public:
        IGUI();
        virtual ~IGUI();

        virtual void init(GraphicsDevice& device) = 0;
        virtual void update(const f32 dt, const i32 xmouse, const i32 ymouse) = 0;
        virtual void draw(CommandList& cmd_list) = 0;
        virtual void shutdown() = 0;
    };
}

#endif // _JOJ_GUI_H