#ifndef _JOJ_BUTTON_H
#define _JOJ_BUTTON_H

#include "core/defines.h"

#include "widget.h"

namespace joj
{
    class JOJ_API Button : public Widget
    {
    public:
        Button();
        Button(const u16 x, const u16 y, const u16 width, const u16 height);
        Button(const u16 x, const u16 y, const u16 width, const u16 height,
            const Color color);
        ~Button();

        virtual ErrorCode create(GraphicsDevice& device) = 0;

        virtual void draw(CommandList& cmd_list) = 0;
    };
}

#endif // _JOJ_BUTTON_H