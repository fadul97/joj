#ifndef _JOJ_EVENT_H
#define _JOJ_EVENT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "event_type.h"
#include "platform/keys.h"

JAPI void event_print();

namespace joj
{
    struct JEvent
    {
        EventType type;

        JEvent();
        virtual ~JEvent();
    };

    struct KeyPressedEvent : public JEvent
    {
        u32 key;

        KeyPressedEvent(const u32 key);
        ~KeyPressedEvent() = default;
    };

    struct KeyReleasedEvent : public JEvent
    {
        u32 key;

        KeyReleasedEvent(const u32 key);
        ~KeyReleasedEvent() = default;
    };

    struct MouseClickedEvent : public JEvent
    {
        Buttons button;

        MouseClickedEvent(const Buttons button);
        ~MouseClickedEvent() = default;
    };

    struct MouseReleasedEvent : public JEvent
    {
        Buttons button;

        MouseReleasedEvent(const Buttons button);
        ~MouseReleasedEvent() = default;
    };

    struct MouseMovedEvent : public JEvent
    {
        i32 x;
        i32 y;

        MouseMovedEvent(const i32 x, const i32 y);
        ~MouseMovedEvent() = default;
    };

    struct WindowCloseEvent : public JEvent
    {
        WindowCloseEvent();
        ~WindowCloseEvent() = default;
    };

    struct WindowResizeEvent : public JEvent
    {
        i32 width;
        i32 height;

        WindowResizeEvent(const i32 width, const i32 height);
        ~WindowResizeEvent() = default;
    };
}

#endif // _JOJ_EVENT_H