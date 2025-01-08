#ifndef _JOJ_JWIDGET_FACTORY_H
#define _JOJ_JWIDGET_FACTORY_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "jbutton.h"
#include <string>

namespace joj
{
    class JAPI JWidgetFactory
    {
    public:
        JWidgetFactory() = default;
        virtual ~JWidgetFactory() = default;

        virtual JButton* create_button(i32 x, i32 y, i32 width, i32 height,
            const std::string& label) = 0;
    };
}

#endif // _JOJ_JWIDGET_FACTORY_H