#ifndef _JOJ_WIN32_WINDOW_DATA_H
#define _JOJ_WIN32_WINDOW_DATA_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "joj/platform/window_mode.h"

namespace joj
{
    struct JOJ_API WindowData
    {
        HWND handle;
        HINSTANCE instance;
        HDC hdc;
        WindowMode window_mode;
        u32 width;
        u32 height;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_WIN32_WINDOW_DATA_H