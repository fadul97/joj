#include "renderer/gui/canvas.h"

#include "logger.h"

joj::Canvas::Canvas()
    : Widget()
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
    Widget* child)
    : Widget(x, y, width, height, child)
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color, Widget* child)
    : Widget(x, y, width, height, color, child)
{
}

joj::Canvas::~Canvas()
{
}