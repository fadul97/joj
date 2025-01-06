#include "renderer/gui/button.h"

joj::Button::Button()
    : IWidget()
{
}

joj::Button::Button(const u16 x, const u16 y, const u16 width, const u16 height)
    : IWidget(x, y, width, height)
{
}

joj::Button::Button(const u16 x, const u16 y, const u16 width, const u16 height, const Color color)
    : IWidget(x, y, width, height, color)
{
}

joj::Button::~Button()
{
}