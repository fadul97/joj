#include "platform/win32/window_win32.h"

#if JPLATFORM_WINDOWS

#include "logger.h"
#include <windowsx.h>

joj::Win32Window::Win32Window()
    : Window{}
{
    m_data.handle = nullptr;
    m_data.hdc = nullptr;
    m_data.window_mode = WindowMode::Windowed;
    m_data.width = 0;
    m_data.height = 0;

    m_icon = LoadIcon(nullptr, IDI_APPLICATION);
    m_cursor = LoadCursor(nullptr, IDC_ARROW);
    m_color = RGB(60, 60, 60);
    m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
}

joj::Win32Window::Win32Window(const char* title, const u32 width, const u32 height,
    const WindowMode mode)
    : Window{title, width, height, mode}
{
    m_data.handle = nullptr;
    m_data.hdc = nullptr;
    m_data.window_mode = mode;
    m_data.width = width;
    m_data.height = height;

    m_icon = LoadIcon(nullptr, IDI_APPLICATION);
    m_cursor = LoadCursor(nullptr, IDC_ARROW);
    m_color = RGB(60, 60, 60);
    m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
}

joj::Win32Window::~Win32Window()
{
    if (m_data.hdc != nullptr)
        ReleaseDC(m_data.handle, m_data.hdc);

    if (m_data.handle != nullptr)
        DestroyWindow(m_data.handle);
}

joj::ErrorCode joj::Win32Window::create()
{
    const char* joj_wnd_class_name = "JOJ_WINDOW_CLASS";

    HINSTANCE app_id = GetModuleHandle(nullptr);
    if (!app_id)
    {
        JFATAL(ErrorCode::ERR_WINDOW_HANDLE, "Failed to get module handle.");
        return ErrorCode::ERR_WINDOW_HANDLE;
    }

    WNDCLASSEX wnd_class;

    if (!GetClassInfoExA(app_id, joj_wnd_class_name, &wnd_class))
    {
        wnd_class.cbSize = sizeof(WNDCLASSEX);
        wnd_class.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wnd_class.lpfnWndProc = WinProc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = app_id;
        wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hbrBackground = CreateSolidBrush(RGB(60, 60, 60));
        wnd_class.lpszMenuName = nullptr;
        wnd_class.lpszClassName = joj_wnd_class_name;
        wnd_class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        // Register "JOJ_WINDOW_CLASS" class
        if (!RegisterClassEx(&wnd_class))
        {
            JERROR(ErrorCode::ERR_WINDOW_REGISTRATION, "Failed to register window class.");
            return ErrorCode::ERR_WINDOW_REGISTRATION;
        }
    }

    // Lock window size to minimum of 200x200
    if (m_width < 200)
        m_width = 200;

    if (m_height < 200)
        m_height = 200;

    if (m_mode == WindowMode::Windowed)
    {
        // If screen and width parameters are higher than Screen size
        i32 screen_width = GetSystemMetrics(SM_CXSCREEN);
        i32 screen_height = GetSystemMetrics(SM_CYSCREEN);

        if (m_width >= screen_width)
        {
            m_data.width = static_cast<u32>(screen_width);
            m_width = static_cast<u32>(screen_width);
        }
        else
        {
            m_data.width = m_width;
        }

        if (m_height >= screen_height)
        {
            m_data.height = static_cast<u32>(screen_height);
            m_height = static_cast<u32>(screen_height);
        }
        else
        {
            m_data.height = m_height;
        }
    }
    else if (m_mode == WindowMode::Fullscreen)
    {
        i32 screen_width = GetSystemMetrics(SM_CXSCREEN);
        i32 screen_height = GetSystemMetrics(SM_CYSCREEN);

        // Ignore width and height paremeters
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
        m_data.width = static_cast<u32>(screen_width);
        m_data.height = static_cast<u32>(screen_height);
        m_width = static_cast<u32>(screen_width);
        m_height = static_cast<u32>(screen_height);
    }
    else
    {
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;

        // If screen and width parameters are higher than Screen size
        i32 screen_width = GetSystemMetrics(SM_CXSCREEN);
        i32 screen_height = GetSystemMetrics(SM_CYSCREEN);

        if (m_width >= screen_width)
        {
            m_data.width = static_cast<u32>(screen_width);
            m_width = static_cast<u32>(screen_width);
        }
        else
        {
            m_data.width = m_width;
        }

        if (m_height >= screen_height)
        {
            m_data.height = static_cast<u32>(screen_height);
            m_height = static_cast<u32>(screen_height);
        }
        else
        {
            m_data.height = m_height;
        }
    }

    m_data.handle = CreateWindowEx(
        0,
        joj_wnd_class_name,
        m_title,
        m_style,
        0, 0,
        m_width, m_height,
        nullptr,
        nullptr,
        app_id,
        nullptr
    );

    if (!m_data.handle)
    {
        JFATAL(ErrorCode::ERR_WINDOW_HANDLE, "Failed to create window.");
        return ErrorCode::ERR_WINDOW_HANDLE;
    }

    // FIXME: Weird size when creating a Fullscreen with Window style,
    //        or a small window with width < 120 with the same options.
    // Maybe something to do with the actual bar width?
    if (m_mode == WindowMode::Windowed)
    {
        RECT new_rect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };

        if (!AdjustWindowRectEx(&new_rect,
            GetWindowStyle(m_data.handle),
            GetMenu(m_data.handle) != nullptr,
            GetWindowExStyle(m_data.handle)))
        {
            JERROR(ErrorCode::ERR_WINDOW_ADJUST, "Could not adjust window rect ex.");
        }

        LONG xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((new_rect.right - new_rect.left) / 2);
        LONG ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((new_rect.bottom - new_rect.top) / 2);

        if (!MoveWindow(
            m_data.handle,
            xpos,
            ypos,
            new_rect.right - new_rect.left,
            new_rect.bottom - new_rect.top,
            TRUE)
            )
        {
            JERROR(ErrorCode::ERR_WINDOW_MOVE, "Could not move window.");
        }
    }
    else if (m_mode == WindowMode::Borderless)
    {
        RECT new_rect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };

        if (!AdjustWindowRectEx(&new_rect,
            GetWindowStyle(m_data.handle),
            GetMenu(m_data.handle) != nullptr,
            GetWindowExStyle(m_data.handle)))
        {
            JERROR(ErrorCode::ERR_WINDOW_ADJUST, "Could not adjust window rect ex.");
        }

        LONG xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((new_rect.right - new_rect.left) / 2);
        LONG ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((new_rect.bottom - new_rect.top) / 2);

        if (!MoveWindow(
            m_data.handle,
            xpos,
            ypos,
            new_rect.right - new_rect.left,
            new_rect.bottom - new_rect.top,
            TRUE)
            )
        {
            JERROR(ErrorCode::ERR_WINDOW_MOVE, "Could not move window.");
        }
    }

    m_data.hdc = GetDC(m_data.handle);
    if (!m_data.hdc)
        JERROR(ErrorCode::ERR_WINDOW_DEVICE_CONTEXT, "Failed to get device context.");

    RECT window_rect;
    if (GetWindowRect(m_data.handle, &window_rect))
    {
        m_window_rect.left = window_rect.left;
        m_window_rect.top = window_rect.top;
        m_window_rect.right = window_rect.right;
        m_window_rect.bottom = window_rect.bottom;
    }
    else
    {
        JERROR(ErrorCode::ERR_WINDOW_RECT, "Failed to get window rect.");
    }

    RECT client_rect;
    if (GetClientRect(m_data.handle, &client_rect))
    {
        m_client_rect.left = client_rect.left;
        m_client_rect.top = client_rect.top;
        m_client_rect.right = client_rect.right;
        m_client_rect.bottom = client_rect.bottom;
    }
    else
    {
        JERROR(ErrorCode::ERR_WINDOW_CLIENT_RECT, "Failed to get client rect.");
    }

    return ErrorCode::OK;
}

void joj::Win32Window::destroy()
{
    if (m_data.hdc != nullptr)
        ReleaseDC(m_data.handle, m_data.hdc);

    if (m_data.handle != nullptr)
        DestroyWindow(m_data.handle);
}

LRESULT CALLBACK joj::Win32Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
    case WM_QUIT:
    case WM_CLOSE:
        JDEBUG("Running = false");
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void joj::Win32Window::get_window_size(u32& width, u32& height)
{
    RECT window_rect;
    if (GetWindowRect(m_data.handle, &window_rect))
    {
        m_window_rect.left = window_rect.left;
        m_window_rect.top = window_rect.top;
        m_window_rect.right = window_rect.right;
        m_window_rect.bottom = window_rect.bottom;

        width = m_window_rect.right - m_window_rect.left;
        height = m_window_rect.bottom - m_window_rect.top;
    }
    else
    {
        JERROR(ErrorCode::ERR_WINDOW_RECT, "Failed to get window rect.");
    }
}

void joj::Win32Window::get_client_size(u32& width, u32& height)
{
    RECT client_rect;
    if (GetClientRect(m_data.handle, &client_rect))
    {
        m_client_rect.left = client_rect.left;
        m_client_rect.top = client_rect.top;
        m_client_rect.right = client_rect.right;
        m_client_rect.bottom = client_rect.bottom;

        width = m_client_rect.right - m_client_rect.left;
        height = m_client_rect.bottom - m_client_rect.top;
    }
    else
    {
        JERROR(ErrorCode::ERR_WINDOW_CLIENT_RECT, "Failed to get client rect.");
    }
}

#endif // JPLATFORM_WINDOWS