#include "joj/platform/input.h"

joj::InputData joj::Input::m_data{};

joj::Input::Input()
{
}

b8 joj::Input::is_key_pressed(const u32 key)
{
    if (m_data.ctrl.keys[key])
    {
        if (is_key_down(key))
        {
            m_data.ctrl.keys[key] = false;
            return true;
        }
    }
    else if (is_key_up(key))
    {
        m_data.ctrl.keys[key] = true;
    }

    return false;
}

b8 joj::Input::is_key_down(const u32 key)
{
    return m_data.keyboard.keys[key];
}

b8 joj::Input::is_key_up(const u32 key)
{
    return !m_data.keyboard.keys[key];
}

b8 joj::Input::is_button_down(Buttons button) const
{
    return m_data.mouse.buttons[button];
}

b8 joj::Input::is_button_up(Buttons button) const
{
    return !m_data.mouse.buttons[button];
}

i16 joj::Input::get_xmouse()
{
    return m_data.mouse.x;
}

i16 joj::Input::get_ymouse()
{
    return m_data.mouse.y;
}

i16 joj::Input::get_mouse_wheel()
{
    return m_data.mouse.wheel;
}