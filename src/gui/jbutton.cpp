#include "gui/jbutton.h"

#include <commctrl.h>
#include "core/logger.h"
#include <Windowsx.h>

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_LBUTTONUP:

        MessageBeep(MB_OK);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RegisterRedPanelClass(void) {

    HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));

    WNDCLASSW rwc = { 0 };

    rwc.lpszClassName = L"RedPanelClass";
    rwc.hbrBackground = hbrush;
    rwc.lpfnWndProc = PanelProc;
    rwc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&rwc);
}

joj::JButton::JButton()
    : JWidget(), m_label("Button"), m_bounds({ 0, 0, 0, 0 }), m_handle({ nullptr })
{
}

joj::JButton::JButton(const u32 x, const u32 y, const u32 width, const u32 height,
    const char* title)
    : JWidget(x, y, width, height, title), m_label(title),
    m_bounds({ x, y, x + width, y + height }), m_handle({ nullptr })
{
}

joj::JButton::~JButton()
{
    if (m_handle.handle)
    {
        unregister_widget(m_handle);
        DestroyWindow(m_handle.handle);
    }
}

static LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
    int id = GetWindowLong(hwnd, GWL_ID);
    JDEBUG("ChildProc: %d", id);

    switch (msg) {
    case WM_LBUTTONDOWN:
        JDEBUG("Button message: %d", msg);
        SetFocus(hwnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON:
            MessageBox(0, "hello world", 0, 0);
            break;
        }
        break;
    }
    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

void joj::JButton::create(JWidgetCreationData& data, const JGUIEvent::Callback& callback)
{
    m_handle.handle = CreateWindowEx(
        0,
        "BUTTON",
        m_label.c_str(),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        m_bounds.left, m_bounds.top,
        m_bounds.right - m_bounds.left,
        m_bounds.bottom - m_bounds.top,
        data.parent_handle,
        HMENU(IDC_BUTTON),
        data.instance,
        nullptr
    );

    if (m_handle.handle)
    {
        register_widget(m_handle);

        s_originalWndProc = (WNDPROC)SetWindowLongPtr(m_handle.handle, GWLP_WNDPROC, (LONG_PTR)ButtonProc);
        if (!s_originalWndProc)
        {
            DWORD error = GetLastError();
            JOJ_FATAL(ErrorCode::FAILED, "SetWindowLongPtr failed with error code: %lu", error);
        }

        if (callback)
            m_callback = callback;

    }
}

void joj::JButton::draw(CommandList& cmd_list)
{
}

void joj::JButton::update(i32 xmouse, i32 ymouse, b8 clicked)
{
    RECT bounds =
    {
        static_cast<LONG>(m_bounds.left),
        static_cast<LONG>(m_bounds.top),
        static_cast<LONG>(m_bounds.right),
        static_cast<LONG>(m_bounds.bottom)
    };

    m_is_hovered = is_hovered(xmouse, ymouse);

    if (m_is_hovered && clicked)
    {
        m_on_click.trigger();
        m_callback();
    }
}

b8 joj::JButton::is_hovered(const i32 x, const i32 y)
{
    return x >= m_x && x <= m_x + m_width && y >= m_y && y <= m_y + m_height;
}

void joj::JButton::on_click(const JGUIEvent::Callback& callback)
{
    if (!m_callback)
    {
        JDEBUG("No callback provided!");
        return;
    }
    else
    {
        JDEBUG("Callback provided!");
        m_callback = callback;
        // m_on_click.set_callback(callback);
    }
}

void joj::JButton::trigger()
{
    if (m_callback)
        m_callback();
}

LRESULT joj::JButton::handle_message(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:
        // JDEBUG("Button message: %d", msg);
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            m_on_click.trigger();
        }
        break;
    }
    return DefWindowProc(m_handle.handle, msg, wParam, lParam);
}

LRESULT CALLBACK joj::JButton::ButtonProc(HWND hWnd, UINT msg, WPARAM wParam,
    LPARAM lParam)
{
    static b8 button_pressed = false;
    HWND parent_handle = GetParent(hWnd);

    switch (msg)
    {
    case WM_LBUTTONDOWN:
        button_pressed = true;
        // SetCapture(hWnd);
        /*
        // JDEBUG("Button message: %d", msg);
        if (parent_handle)
        {
            // Enviar mensagem para a janela pai
            SendMessage(parent_handle, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), BN_CLICKED), (LPARAM)hWnd);
        }
        */
        break;
    case WM_LBUTTONUP:
        // ReleaseCapture();
        if (button_pressed)
        {
            button_pressed = false;
            /*
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            RECT rect;
            GetClientRect(hWnd, &rect);
            if (PtInRect(&rect, pt) && parent_handle)
            {
                SendMessage(parent_handle, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), BN_CLICKED), (LPARAM)hWnd);
            }
            */
            /*
            if (parent_handle)
            {
                SendMessage(parent_handle, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), BN_CLICKED), (LPARAM)hWnd);
            }
            */
        }
        break;
        /*
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            // JDEBUG("Button clicked");
        }
        break;
        */
    default:
        break;
    }

    // Chamar o procedimento de janela original do bot�o para manter visibilidade e funcionalidade
    return CallWindowProc(s_originalWndProc, hWnd, msg, wParam, lParam);
}

