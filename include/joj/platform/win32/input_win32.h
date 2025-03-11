#ifndef _JOJ_WIN32_INPUT_H
#define _JOJ_WIN32_INPUT_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "platform/input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace joj
{
    class JOJ_API Win32Input : public Input
    {
    public:
        Win32Input();
        ~Win32Input() = default;

        void set_window(WindowData window);

        static LRESULT CALLBACK InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_WIN32_INPUT_H