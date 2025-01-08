#ifndef _JOJ_EVENT_H
#define _JOJ_EVENT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"
#include <functional>

namespace joj
{
    class JAPI JEvent
    {
    public:
    public:
        using Callback = std::function<void()>;
        
        JEvent();
        virtual ~JEvent() = default;

        void set_callback(Callback callback);
        void trigger();

    private:
        Callback m_callback;
    };

    inline void JEvent::set_callback(Callback callback)
    {
        m_callback = callback;
    }

    inline void JEvent::trigger()
    {
        if (m_callback)
            m_callback();
    }
}

#endif // _JOJ_EVENT_H