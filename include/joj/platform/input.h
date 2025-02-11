#ifndef _JOJ_INPUT_H
#define _JOJ_INPUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "keys.h"
#include "window.h"

namespace joj
{
    struct InputData
    {
        Keyboard keyboard;
        Keyboard ctrl;
        Mouse mouse;
    };

    class Input
    {
    public:
        Input();
        virtual ~Input();

        virtual void set_window(WindowData window) = 0;

        b8 is_key_down(u32 key);
        b8 is_key_pressed(u32 key);
        b8 is_key_up(u32 key);

        b8 is_button_down(Buttons button) const;
        b8 is_button_up(Buttons button) const;

        i16 get_xmouse();
        i16 get_ymouse();
        i16 get_mouse_wheel();

    protected:
        static InputData m_data;
    };

    inline b8 Input::is_key_down(const u32 key)
    { return m_data.keyboard.keys[key]; }

    inline b8 Input::is_key_up(const u32 key)
    { return !m_data.keyboard.keys[key]; }

    inline b8 Input::is_button_down(Buttons button) const
    { return m_data.mouse.buttons[button]; }

    inline b8 Input::is_button_up(Buttons button) const
    { return !m_data.mouse.buttons[button]; }

    inline i16 Input::get_xmouse()
    { return m_data.mouse.x; }

    inline i16 Input::get_ymouse()
    { return m_data.mouse.y; }

    inline i16 Input::get_mouse_wheel()
    { return m_data.mouse.wheel; }
}

#endif // _JOJ_INPUT_H