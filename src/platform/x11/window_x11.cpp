#include "platform/x11/window_x11.h"

#if JPLATFORM_LINUX

joj::X11Window::X11Window()
    : Window{}
{
    m_data.handle = nullptr;
    m_data.instance = 0;
    m_data.screen = nullptr;
    m_data.visual_info = nullptr;
    m_data.window_mode = WindowMode::Windowed;
    m_data.width = 0;
    m_data.height = 0;

    // TODO: Set default color
    // m_color = RGB(60, 60, 60);

    // TODO: Set default styles
    // m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
}

joj::X11Window::X11Window(const char* title, const u32 width, const u32 height,
    const WindowMode mode)
    : Window{title, width, height, mode}
{
    m_data.handle = nullptr;
    m_data.instance = 0;
    m_data.screen = nullptr;
    m_data.visual_info = nullptr;
    m_data.window_mode = mode;
    m_data.width = width;
    m_data.height = height;

    // TODO: Set default color
    // m_color = RGB(60, 60, 60);

    // TODO: Set default styles
    // m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
}

// TODO: Implement destructor
joj::X11Window::~X11Window()
{
}

joj::ErrorCode joj::X11Window::create()
{
    // JTODO();
    return ErrorCode::FAILED;
}

// TODO: Implement destroy method
void joj::X11Window::destroy()
{
}

void joj::X11Window::get_window_size(u32& width, u32& height)
{
    // JTODO();
}

void joj::X11Window::get_client_size(u32& width, u32& height)
{
    // JTODO();
}

void joj::X11Window::set_title(const char* title)
{
    // JTODO();
}

#endif // JPLATFORM_LINUX