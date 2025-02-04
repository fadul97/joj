#ifndef _JOJ_X11_WINDOW_H
#define _JOJ_X11_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace joj
{
    struct WindowData
    {
        Display* handle;
        u32 instance;
        Screen* screen;
        XVisualInfo* visual_info;
        WindowMode window_mode;
        u32 width;
        u32 height;
    };

    class JAPI X11Window : public Window<WindowData>
    {
    public:
        X11Window();
        X11Window(const char* title, const u32 width, const u32 height,
            const WindowMode mode);
        ~X11Window();

        ErrorCode create() override;
        void destroy() override;

        void get_window_size(u32& width, u32& height) override;
        void get_client_size(u32& width, u32& height) override;

        void set_title(const char* title) override;

    private:
        u32 m_color;
        
        // TODO: Implement styles
        // WindowStyles m_styles;
    };
}

#endif // JPLATFORM_LINUX

#endif // _JOJ_X11_WINDOW_H