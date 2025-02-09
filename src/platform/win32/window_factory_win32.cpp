#include "platform/win32/window_factory_win32.h"

#if JPLATFORM_WINDOWS

#include "platform/window_registration_class.h"
#include "core/logger.h"
#include "core/jmacros.h"

joj::Win32WindowFactory::Win32WindowFactory()
    : WindowFactory<Win32Window>()
{
}

joj::Win32WindowFactory::~Win32WindowFactory()
{
}

joj::Win32Window* joj::Win32WindowFactory::create_window(const char* title, const u32 width,
    const u32 height, const WindowMode mode)
{
    Win32Window* window = new Win32Window(title, width, height, mode);
    if JOJ_FAILED(window->create())
    {
        delete window;
        return nullptr;
    }

    return window;
}

joj::ErrorCode joj::Win32WindowFactory::create_window_class(WindowRegistrationClass& window_class)
{
    const char* class_name = window_class.class_name.c_str();

    HINSTANCE app_id = static_cast<HINSTANCE>(window_class.instance);

    DWORD styles = convert_window_styles(window_class.styles);

    WNDCLASSEX wnd_class = { 0 };
    if (!GetClassInfoExA(app_id, class_name, &wnd_class))
    {
        wnd_class.cbSize = sizeof(WNDCLASSEX);
        wnd_class.style = styles;
        wnd_class.lpfnWndProc = (WNDPROC)window_class.procedure;
        wnd_class.hInstance = app_id;
        wnd_class.lpszMenuName = window_class.menu_name.c_str();
        wnd_class.lpszClassName = class_name;
        // wnd_class.lpfnWndProc = WinProc;
        wnd_class.lpszMenuName = nullptr;
        // wnd_class.lpszClassName = joj_wnd_class_name;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hbrBackground = CreateSolidBrush(RGB(60, 60, 60));
        wnd_class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        
        if (!RegisterClassEx(&wnd_class))
        {
            DWORD error_code = GetLastError();
            char error_message[256];
            FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                error_code,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                error_message,
                sizeof(error_message),
                nullptr
            );

            JFATAL(ErrorCode::ERR_WINDOW_REGISTRATION,
                "Failed to register window class. Error %lu: %s", error_code, error_message);
            return ErrorCode::ERR_WINDOW_REGISTRATION;
        }
    }

    return ErrorCode::OK;
}

DWORD joj::Win32WindowFactory::convert_window_styles(WindowStyles styles)
{
    DWORD result = 0;

    if (static_cast<u32>(styles & WindowStyles::DoubleClick))
        result |= CS_DBLCLKS;

    if (static_cast<u32>(styles & WindowStyles::OwnDC))
        result |= CS_OWNDC;

    if (static_cast<u32>(styles & WindowStyles::Horizontal))
        result |= CS_HREDRAW;

    if (static_cast<u32>(styles & WindowStyles::Vertical))
        result |= CS_VREDRAW;

    if (static_cast<u32>(styles & WindowStyles::Child))
        result |= WS_CHILD;

    return result;
}

#endif // JPLATFORM_WINDOWS