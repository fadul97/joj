#include "gui/jwidget.h"

joj::JWidget::WidgetMap joj::JWidget::g_widget_map;

joj::JWidget::JWidget()
    : m_x(0), m_y(0), m_width(0), m_height(0), m_on_click(), m_is_hovered(false)
{
}

joj::JWidget::JWidget(const u32 x, const u32 y, const u32 width, const u32 height,
    const char* title)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_on_click(), m_is_hovered(false)
{
}