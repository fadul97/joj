#include "gui/jbutton.h"

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
}

void joj::JButton::create(JWidgetCreationData& data)
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
        nullptr,
        data.instance,
        nullptr
    );
}

void joj::JButton::draw(CommandList& cmd_list)
{
}

void joj::JButton::update(i32 xmouse, i32 ymouse, b8 clicked)
{
}

