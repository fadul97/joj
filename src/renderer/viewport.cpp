#include "joj/renderer/viewport.h"

joj::Viewport::Viewport()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
    m_min_depth = 0.0f;
    m_max_depth = 0.0f;
}

joj::Viewport::Viewport(f32 x, f32 y, f32 width, f32 height, f32 min_depth, f32 max_depth)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_min_depth = min_depth;
    m_max_depth = max_depth;
}

joj::Viewport::~Viewport()
{
}

void joj::Viewport::set(f32 x, f32 y, f32 width, f32 height, f32 min_depth, f32 max_depth)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_min_depth = min_depth;
    m_max_depth = max_depth;
}

void joj::Viewport::set_size(const u32 width, const u32 height)
{
    m_width = static_cast<f32>(width);
    m_height = static_cast<f32>(height);
}