#include "renderer/d3d11/gui/gui_d3d11.h"

#if JPLATFORM_WINDOWS

#include "jmacros.h"
#include "logger.h"
// #include "renderer/d3d11/renderer_d3d11.h"
#include "platform/win32/window_win32.h"
#include <windowsx.h>

joj::IRenderer* joj::D3D11GUI::s_renderer = nullptr;

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
        if (!RegisterClassEx(&wnd_class))
        {
            JERROR(ErrorCode::ERR_WINDOW_REGISTRATION, "Failed to register window class.");
            return;
        }
    }

    u32 width = window.width / 4;
    u32 height = window.height - 100;
    DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

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
        JFATAL(ErrorCode::ERR_WINDOW_HANDLE, "Failed to create window.");
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

    m_initialized = true;
}

void joj::D3D11GUI::update(const f32 dt, const i32 xmouse, const i32 ymouse, const b8 clicked)
{
    if (m_initialized == false)
    {
        JERROR(ErrorCode::ERR_GUI_NOT_INITIALIZED, "GUI not initialized.");
        return;
    }

    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it)
    {
        auto& widget = *it;
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
    const std::string& label)
{
    JButton* button = m_factory.create_button(10, 10, 100, 30, "Click Me!");
    if (!button)
    {
        JFATAL(ErrorCode::ERR_GUI_BUTTON_WIN32_CREATION, "Failed to create button.");
        return;
    }

    m_widgets.push_back(button);
}

LRESULT CALLBACK joj::D3D11GUI::GUIWinProc(HWND hWnd, UINT msg, WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
        // Show Window if Space is pressed
    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            ShowWindow(hWnd, SW_HIDE);
        }
        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
        {
            JDEBUG("Button clicked");
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
    case WM_QUIT:
    case WM_CLOSE:
        JDEBUG("Running = false");
        ShowWindow(hWnd, SW_HIDE);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif // JPLATFORM_WINDOWS