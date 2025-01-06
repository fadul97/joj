#include "renderer/gui/canvas.h"

#include "logger.h"

joj::Canvas::Canvas()
    : m_x(0), m_y(0), m_width(0), m_height(0),
    m_background_color(Color(0.0f, 0.0f, 0.0f, 1.0f)),
    m_normalized_x(0), m_normalized_y(0),
    m_normalized_width(0), m_normalized_height(0)
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_background_color(Color(0.0f, 0.0f, 0.0f, 1.0f))
{
    const f32 window_width = 800.0f;
    const f32 window_height = 600.0f;

    m_normalized_x = 2.0f * static_cast<f32>(x) / window_width - 1.0f;
    m_normalized_y = 1.0f - 2.0f * static_cast<f32>(y) / window_height;

    m_normalized_width = 2.0f * static_cast<f32>(width) / window_width;
    m_normalized_height = 2.0f * static_cast<f32>(height) / window_height;

    JDEBUG("Canvas size: %f, %f, %f, %f",
        m_normalized_x, m_normalized_y, m_normalized_width, m_normalized_height);
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_background_color(color)
{
    const f32 window_width = 800.0f;
    const f32 window_height = 600.0f;

    m_normalized_x = 2.0f * static_cast<f32>(x) / window_width - 1.0f;
    m_normalized_y = 1.0f - 2.0f * static_cast<f32>(y) / window_height;

    m_normalized_width = 2.0f * static_cast<f32>(width) / window_width;
    m_normalized_height = 2.0f * static_cast<f32>(height) / window_height;

    JDEBUG("Canvas size: %f, %f, %f, %f",
        m_normalized_x, m_normalized_y, m_normalized_width, m_normalized_height);
}

joj::Canvas::~Canvas()
{
}