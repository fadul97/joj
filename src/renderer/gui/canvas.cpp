#include "joj/renderer/gui/canvas.h"

#include "joj/core/logger.h"

joj::Canvas::Canvas()
    : Widget()
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height)
    : Widget(x, y, width, height)
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color)
    : Widget(x, y, width, height, color)
{
}

joj::Canvas::~Canvas()
{
}