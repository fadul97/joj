#ifndef _JOJ_EVENT_H
#define _JOJ_EVENT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"
#include <functional>
#include "core/logger.h"

namespace joj
{
    class JAPI JEvent
    {
    public:
        using Callback = std::function<void()>;
        
        JEvent();
        virtual ~JEvent() = default;

        void set_callback(const Callback& callback);
        void trigger();

    private:
        Callback m_callback;
    };

    inline void JEvent::set_callback(const Callback& callback)
    {
        m_callback = callback;
    }

    inline void JEvent::trigger()
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

#endif // _JOJ_EVENT_H