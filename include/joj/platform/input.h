#ifndef _JOJ_INPUT_H
#define _JOJ_INPUT_H

#include "joj/core/defines.h"

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

    class JOJ_API Input
    {
    public:
        Input();
        virtual ~Input() = default;

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
}

#endif // _JOJ_INPUT_H