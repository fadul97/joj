#ifndef _JOJ_WIN32_WINDOW_H
#define _JOJ_WIN32_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/window.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace joj
{
    struct WindowData
    {
        HWND handle;
        HDC hdc;
        WindowMode window_mode;
        u16 width;
        u16 height;
    };

    class JAPI Win32Window : Window<WindowData>
    {
    public:
        Win32Window();
        Win32Window(const char* title, const u32 width, const u32 height);
        ~Win32Window();

    private:
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_WIN32_WINDOW_H