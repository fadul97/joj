#include "renderer/gui/canvas.h"

#include "logger.h"

joj::Canvas::Canvas()
    : IWidget()
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height)
    : IWidget(x, y, width, height)
{
}

joj::Canvas::Canvas(const u16 x, const u16 y, const u16 width, const u16 height, const Color color)
    : IWidget(x, y, width, height, color)
{
}

joj::Canvas::~Canvas()
{
}