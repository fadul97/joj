#include "renderer/d3d11/gui/gui_d3d11.h"

#if JPLATFORM_WINDOWS

#include "jmacros.h"
#include "logger.h"
// #include "renderer/d3d11/renderer_d3d11.h"
#include "platform/win32/window_win32.h"
#include <windowsx.h>

joj::IRenderer* joj::D3D11GUI::s_renderer = nullptr;
joj::Mouse joj::D3D11GUI::s_mouse = { 0 };

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

    m_main_window.handle = window.handle;
    m_main_window.hdc = window.hdc;
    m_main_window.window_mode = window.window_mode;
    m_main_window.width = window.width;
    m_main_window.height = window.height;

    HINSTANCE app_id = window.instance;

    // ---------------------------------------------------
    // Create Button for Main Window
    // ---------------------------------------------------

    ParentData parent_data = { m_main_window.handle, app_id };
    m_factory = JWin32WidgetFactory(parent_data);

    // Create a button
    JButton* button = m_factory.create_button(10, 10, 100, 30, "Click Me!");
    if (!button)
    {
        JFATAL(ErrorCode::ERR_GUI_BUTTON_WIN32_CREATION, "Failed to create button.");
        return;
    }

    m_widgets.push_back(button);

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
        // JDEBUG("Clicked: %d", clicked);
        widget->update(xmouse, ymouse, clicked);
        if (widget->is_hovered(s_mouse.x, s_mouse.y))
        {
            // JDEBUG("Mouse hovered");
        }
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
        button->on_click(callback);

    m_widgets.push_back(button);
}

LRESULT CALLBACK joj::D3D11GUI::GUIWinProc(HWND hWnd, UINT msg, WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
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