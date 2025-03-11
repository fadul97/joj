#ifndef _JOJ_CANVAS_H
#define _JOJ_CANVAS_H

#include "core/defines.h"

#include "widget.h"

namespace joj
{
    class JAPI Canvas : public Widget
    {
    public:
        Canvas();
        Canvas(const u16 x, const u16 y, const u16 width, const u16 height);
        Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        ~Canvas();

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void draw(CommandList& cmd_list) = 0;
    };
}
#endif // _JOJ_CANVAS_H