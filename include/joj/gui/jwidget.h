#ifndef _JOJ_JWIDGET_H
#define _JOJ_JWIDGET_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    struct JWidgetCreationData;
    struct CommandList;

    class JAPI JWidget
    {
    public:
        JWidget();
        virtual ~JWidget() = default;

        virtual void create(JWidgetCreationData& data) = 0;
        virtual void draw(CommandList& cmd_list) = 0;
        virtual void update(i32 xmouse, i32 ymouse, b8 clicked) = 0;
    };
}

#endif // _JOJ_JWIDGET_H