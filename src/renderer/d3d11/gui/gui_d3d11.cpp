#include "renderer/d3d11/gui/gui_d3d11.h"

#if JPLATFORM_WINDOWS

#include "jmacros.h"
#include "logger.h"
// #include "renderer/d3d11/renderer_d3d11.h"
#include "platform/win32/window_win32.h"
#include <windowsx.h>
#include "platform/win32/input_win32.h"

joj::IRenderer* joj::D3D11GUI::s_renderer = nullptr;
joj::Mouse joj::D3D11GUI::s_mouse = { 0 };
WNDPROC joj::D3D11GUI::s_originalWndProc = nullptr;

#define IDC_BUTTON1 2010

joj::D3D11GUI::D3D11GUI()
    : m_initialized(false)
{
    m_main_window.handle = nullptr;
    m_main_window.hdc = nullptr;
    m_main_window.window_mode = WindowMode::Windowed;
    m_main_window.width = 0;
    m_main_window.height = 0;
}

joj::D3D11GUI::~D3D11GUI()
{
}

void joj::D3D11GUI::init(WindowData& window, IRenderer& renderer)
{
    s_renderer = &renderer;

    /*
    m_main_window.handle = window.handle;
    m_main_window.hdc = window.hdc;
    m_main_window.window_mode = window.window_mode;
    m_main_window.width = window.width;
    m_main_window.height = window.height;

    HINSTANCE app_id = window.instance;
    */

    // ---------------------------------------------------
    // Create child window of Window
    // ---------------------------------------------------

    const char* gui_class_name = "JGUI_WINDOW_CLASS";

    HINSTANCE app_id = GetModuleHandle(nullptr);
    if (!app_id)
    {
        JFATAL(ErrorCode::ERR_WINDOW_HANDLE, "Failed to get module handle.");
        return;
    }

    WNDCLASSEX wnd_class;
    if (!GetClassInfoExA(app_id, gui_class_name, &wnd_class))
    {
        wnd_class.cbSize = sizeof(WNDCLASSEX);
        wnd_class.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wnd_class.lpfnWndProc = GUIWinProc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = app_id;
        wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hbrBackground = CreateSolidBrush(RGB(60, 60, 60));
        wnd_class.lpszMenuName = nullptr;
        wnd_class.lpszClassName = gui_class_name;
        wnd_class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        // Register "JOJ_WINDOW_CLASS" class
        if (RegisterClassEx(&wnd_class) != TRUE)
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

            JERROR(ErrorCode::ERR_WINDOW_REGISTRATION, "Failed to register window class. Error %lu: %s",
                error_code, error_message);
            // return;
        }
    }

    if (!GetClassInfoExA(app_id, gui_class_name, &wnd_class))
    {
        DWORD error_code = GetLastError();
        JFATAL(ErrorCode::ERR_WINDOW_REGISTRATION,
            "Class not found after registration. Error %lu", error_code);
        return;
    }
    else
    {
        JINFO("Class already registered.");
    }

    u32 width = window.width / 4;
    u32 height = window.height - 100;
    DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

    JDEBUG("Using class name: %s", gui_class_name);
    m_main_window.handle = CreateWindowEx(
        0,
        gui_class_name,
        "GUIMainWindow",
        style,
        window.width - width, 0,
        width, height,
        window.handle,
        nullptr,
        app_id,
        nullptr
    );

    if (!m_main_window.handle)
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

        JFATAL(ErrorCode::ERR_WINDOW_HANDLE,
            "Failed to create window. Error %lu: %s", error_code, error_message);
        return;
    }

    RECT new_rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    if (!AdjustWindowRectEx(&new_rect,
        GetWindowStyle(m_main_window.handle),
        GetMenu(m_main_window.handle) != nullptr,
        GetWindowExStyle(m_main_window.handle)))
    {
        JERROR(ErrorCode::ERR_WINDOW_ADJUST, "Could not adjust window rect ex.");
    }

    m_main_window.x = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((new_rect.right - new_rect.left) / 2);
    m_main_window.y = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((new_rect.bottom - new_rect.top) / 2);
    if (!MoveWindow(
        m_main_window.handle,
        m_main_window.x + window.width / 3,
        m_main_window.y + 10,
        new_rect.right - new_rect.left,
        new_rect.bottom - new_rect.top,
        TRUE))
    {
        JERROR(ErrorCode::ERR_WINDOW_MOVE, "Could not move window.");
    }

    m_main_window.hdc = GetDC(m_main_window.handle);
    if (!m_main_window.hdc)
    {
        JERROR(ErrorCode::ERR_WINDOW_DEVICE_CONTEXT, "Failed to get device context.");
        return;
    }

    // ---------------------------------------------------
    // Create Button for Main Window
    // ---------------------------------------------------

    ParentData parent_data = { m_main_window.handle, app_id };
    m_factory = JWin32WidgetFactory(parent_data);

    // SetWindowLongPtr(m_main_window.handle, GWLP_WNDPROC, (LONG_PTR)GUIWinProc);
    s_originalWndProc = (WNDPROC)SetWindowLongPtr(m_main_window.handle, GWLP_WNDPROC, (LONG_PTR)GUIWinProc);

    // Log Handle
    JDEBUG("GUI Handle: %p", m_main_window.handle);

    m_initialized = true;
}

static f32 time_elaped = 0.0f;

void joj::D3D11GUI::update(const f32 dt, const i32 xmouse, const i32 ymouse, const b8 clicked)
{
    if (m_initialized == false)
    {
        JERROR(ErrorCode::ERR_GUI_NOT_INITIALIZED, "GUI not initialized.");
        return;
    }

    time_elaped += dt;

    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it)
    {
        auto& widget = *it;
        // JDEBUG("Clicked: %d", clicked);
        widget->update(xmouse, ymouse, clicked);
        if (widget->is_hovered(s_mouse.x, s_mouse.y) && clicked)
        {
            // JDEBUG("Mouse hovered");
        }
    }

    for (auto it = JWidget::get_widget_map().begin(); it != JWidget::get_widget_map().end(); ++it)
    {
        auto& widget = it->second;
        widget->update(xmouse, ymouse, clicked);
    }
}

void joj::D3D11GUI::draw(CommandList& cmd_list)
{
    if (m_initialized == false)
    {
        JERROR(ErrorCode::ERR_GUI_NOT_INITIALIZED, "GUI not initialized.");
        return;
    }

    for (auto& widget : m_widgets)
    {
        widget->draw(cmd_list);
    }
}

void joj::D3D11GUI::shutdown()
{
    if (m_initialized == false)
    {
        JERROR(ErrorCode::ERR_GUI_NOT_INITIALIZED, "GUI not initialized.");
        return;
    }

    for (auto& widget : m_widgets)
    {
        delete widget;
    }

    if (m_main_window.hdc != nullptr)
        ReleaseDC(m_main_window.handle, m_main_window.hdc);

    if (m_main_window.handle != nullptr)
        DestroyWindow(m_main_window.handle);
}

void joj::D3D11GUI::add_widget(JWidget* widget)
{
    m_widgets.push_back(widget);
}

void joj::D3D11GUI::add_button(i32 x, i32 y, i32 width, i32 height,
    const std::string& label, const JEvent::Callback& callback)
{
    JButton* button = m_factory.create_button(10, 10, 100, 30, "Click Me!");
    if (!button)
    {
        JFATAL(ErrorCode::ERR_GUI_BUTTON_WIN32_CREATION, "Failed to create button.");
        return;
    }

    if (callback)
    {
        JDEBUG("Setting callback for button");
        button->on_click(callback);
        button->set_callback(callback);
    }
    else
    {
        JDEBUG("Callback is null when adding button!");
    }

    // m_widgets.push_back(button);
}

void joj::D3D11GUI::add_button(const std::string& label, i32 x, i32 y, i32 width, i32 height,
    const std::function<void()>& function)
{
    JButton* button = m_factory.create_button(10, 10, 100, 30, "Click Me!", function);
    if (!button)
    {
        JFATAL(ErrorCode::ERR_GUI_BUTTON_WIN32_CREATION, "Failed to create button.");
        return;
    }

    button->trigger();
    JDEBUG("Called trigger!");
    
    // m_widgets2.push_back(std::move(button));
    
}


LRESULT CALLBACK joj::D3D11GUI::GUIWinProc(HWND hWnd, UINT msg, WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        int control_id = LOWORD(wParam); // ID do controle
        int notification_code = HIWORD(wParam); // Código de notificação
        HWND sender_handle = (HWND)lParam; // Handle do controle que enviou

        if (notification_code == BN_CLICKED)
        {
            // JDEBUG("Button clicked! Control ID: %d, Handle: %p", control_id, sender_handle);
            // Você pode procurar o widget no mapa aqui se necessário
            auto it = JWidget::get_widget_map().find(sender_handle);
            if (it != JWidget::get_widget_map().end())
            {
                auto widget = it->second;
                JDEBUG("Widget found in map. Address: %p", widget);
                widget->trigger();
                // return widget->handle_message(msg, wParam, lParam);
                return 0;
            }
            else
            {
                JDEBUG("Widget not found in map for handle: %p", sender_handle);
            }
        }
        break;
    }
    // Read Mouse Position
    case WM_MOUSEMOVE:
    {
        s_mouse.x = GET_X_LPARAM(lParam);
        s_mouse.y = GET_Y_LPARAM(lParam);
        return 0;
    }
    // Read Mouse Button
    case WM_LBUTTONDOWN:
    {
        s_mouse.buttons[BUTTON_LEFT] = true;
        return 0;
    }
    case WM_LBUTTONUP:
    {
        s_mouse.buttons[BUTTON_LEFT] = false;
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        s_mouse.buttons[BUTTON_MIDDLE] = true;
        return 0;
    }
    case WM_MBUTTONUP:
    {
        s_mouse.buttons[BUTTON_MIDDLE] = false;
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        s_mouse.buttons[BUTTON_RIGHT] = true;
        return 0;
    }
    case WM_RBUTTONUP:
    {
        s_mouse.buttons[BUTTON_RIGHT] = false;
        return 0;
    }
    // Hide Window if Space is pressed
    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            ShowWindow(hWnd, SW_HIDE);
        }
        break;
    }

    case WM_DESTROY:
    case WM_QUIT:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    return CallWindowProc(s_originalWndProc, hWnd, msg, wParam, lParam);
    // return DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif // JPLATFORM_WINDOWS