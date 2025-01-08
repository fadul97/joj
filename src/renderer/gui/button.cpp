#include "renderer/gui/button.h"

joj::Button::Button()
    : Widget()
{
}

joj::Button::Button(const u16 x, const u16 y, const u16 width, const u16 height)
    : Widget(x, y, width, height)
{
}

joj::Button::Button(const u16 x, const u16 y, const u16 width, const u16 height,
    const Color color)
    : Widget(x, y, width, height, color)
{
}

joj::Button::~Button()
{
}