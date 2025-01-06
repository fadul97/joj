#ifndef _JOJ_GUI_H
#define _JOJ_GUI_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    class JAPI IGUI
    {
    public:
        IGUI();
        virtual ~IGUI();
        
        virtual void init() = 0;
        virtual void update(const f32 dt) = 0;
        virtual void draw() = 0;
        virtual void shutdown() = 0;
    }
}

#endif // _JOJ_GUI_H