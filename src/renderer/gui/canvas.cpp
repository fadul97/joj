#include "renderer/gui/canvas.h"

joj::Canvas::Canvas()
    : m_x(0), m_y(0), m_width(0), m_height(0),
    m_background_color(Color(0.0f, 0.0f, 0.0f, 1.0f))
{
}

joj::Canvas::Canvas(u16 x, u16 y, u16 width, u16 height)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_background_color(Color(0.0f, 0.0f, 0.0f, 1.0f))
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_background_color(color)
{
}

joj::Canvas::~Canvas()
{
}