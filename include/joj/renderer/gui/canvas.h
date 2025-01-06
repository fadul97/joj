#ifndef _JOJ_CANVAS_H
#define _JOJ_CANVAS_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    class JAPI Canvas
    {
    public:
        Canvas();
        ~Canvas();

    protected:
        u16 m_x;
        u16 m_y;
        u16 m_width;
        u16 m_height;
    };
}
#endif // _JOJ_CANVAS_H