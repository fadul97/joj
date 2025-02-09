#ifndef _JOJ_EVENT_TYPE_H
#define _JOJ_EVENT_TYPE_H

namespace joj
{
    enum class EventType
    {
        Unknown,
        KeyPressed,
        KeyReleased,
        MouseClicked,
        MouseReleased,
        MouseMoved,
        WindowClose,
        WindowResize,
    };
}

#endif // _JOJ_EVENT_TYPE_H