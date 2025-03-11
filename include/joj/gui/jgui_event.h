#ifndef _JOJ_GUI_EVENT_H
#define _JOJ_GUI_EVENT_H

#include "joj/core/defines.h"
#include <functional>
#include "joj/core/logger.h"

namespace joj
{
    class JOJ_API JGUIEvent
    {
    public:
        using Callback = std::function<void()>;
        
        JGUIEvent();
        virtual ~JGUIEvent() = default;

        void set_callback(const Callback& callback);
        void trigger();

    private:
        Callback m_callback;
    };

    inline void JGUIEvent::set_callback(const Callback& callback)
    {
        m_callback = callback;
    }

    inline void JGUIEvent::trigger()
    {
        if (m_callback)
        {
            JDEBUG("Executing callback...");
            m_callback();
        }
        else
        {
            JDEBUG("Callback is null!");
        }
    }
}

#endif // _JOJ_GUI_EVENT_H