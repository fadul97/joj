#ifndef _JOJ_WINDOW_REGISTRATION_CLASS_H
#define _JOJ_WINDOW_REGISTRATION_CLASS_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "window_procedure.h"
#include <string>

namespace joj
{
    enum class WindowStyles
    {
        None = 0,
        DoubleClick = 1 << 0, // CS_DBLCLKS
        OwnDC = 1 << 1, // CS_OWNDC
        Horizontal = 1 << 2, // CS_HREDRAW
        Vertical = 1 << 3, // CS_VREDRAW
        Child = 1 << 4, // WS_CHILD
    };

    inline WindowStyles operator&(WindowStyles a, WindowStyles b)
    {
        using T = std::underlying_type_t<WindowStyles>;
        return static_cast<WindowStyles>(static_cast<T>(a) & static_cast<T>(b));
    }

    inline WindowStyles operator|(WindowStyles a, WindowStyles b)
    {
        return static_cast<WindowStyles>(
            static_cast<u32>(a) | static_cast<u32>(b)
        );
    }

    inline bool has_flag(WindowStyles styles, WindowStyles flag)
    {
        return (static_cast<u32>(styles) & static_cast<u32>(flag)) != 0;
    }

    struct WindowRegistrationClass
    {
        WindowStyles styles = WindowStyles::None;
        void* procedure = nullptr;
        void* instance = nullptr;
        std::string menu_name;
        std::string class_name;
    };
}

#endif // _JOJ_WINDOW_REGISTRATION_CLASS_H