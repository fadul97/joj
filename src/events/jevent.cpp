#include "events/jevent.h"

#include <stdio.h>

void event_print()
{
    printf("Hello from %s jojEvents!\n",
#ifdef JOJ_BUILDING_DLL
        "shared"
#else
        "static"
#endif
    );
}

joj::JEvent::JEvent()
    : type(EventType::Unknown)
{
}

joj::JEvent::~JEvent()
{
}

joj::KeyPressedEvent::KeyPressedEvent(const u32 key)
    : key(key)
{
    type = EventType::KeyPressed;
}

joj::KeyReleasedEvent::KeyReleasedEvent(const u32 key)
    : key(key)
{
    type = EventType::KeyReleased;
}

joj::MouseClickedEvent::MouseClickedEvent(const Buttons button)
    : button(button)
{
    type = EventType::MouseClicked;
}

joj::MouseReleasedEvent::MouseReleasedEvent(const Buttons button)
    : button(button)
{
    type = EventType::MouseReleased;
}

joj::MouseMovedEvent::MouseMovedEvent(const i32 x, const i32 y)
    : x(x), y(y)
{
    type = EventType::MouseMoved;
}

joj::WindowCloseEvent::WindowCloseEvent()
{
    type = EventType::WindowClose;
}

joj::WindowResizeEvent::WindowResizeEvent(const i32 width, const i32 height)
    : width(width), height(height)
{
    type = EventType::WindowResize;
}