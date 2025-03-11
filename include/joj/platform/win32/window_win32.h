#ifndef _JOJ_WIN32_WINDOW_H
#define _JOJ_WIN32_WINDOW_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "platform/window.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace joj
{
    struct WindowData
    {
        HWND handle;
        HINSTANCE instance;
        HDC hdc;
        WindowMode window_mode;
        u32 width;
        u32 height;
    };

    class JOJ_API Win32Window : public Window<WindowData>
    {
    public:
        Win32Window();
        Win32Window(const char* title, const u32 width, const u32 height,
            const WindowMode mode);
        ~Win32Window();

        ErrorCode create() override;
        void destroy() override;

        void get_window_size(u32& width, u32& height) override;
        void get_client_size(u32& width, u32& height) override;

        void set_title(const char* title) override;

        static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam,
            LPARAM lParam);

    private:
        HICON m_icon;
        HCURSOR m_cursor;
        u32 m_color;
        u32 m_style;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_WIN32_WINDOW_H