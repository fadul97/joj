#include "platform/input.h"

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