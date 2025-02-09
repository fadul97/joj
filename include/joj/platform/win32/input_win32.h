#ifndef _JOJ_WIN32_INPUT_H
#define _JOJ_WIN32_INPUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "platform/input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace joj
{
    class JAPI Win32Input : public Input
    {
    public:
        Win32Input();
        ~Win32Input();

        void set_window(WindowData window);

        static LRESULT CALLBACK InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_WIN32_INPUT_H