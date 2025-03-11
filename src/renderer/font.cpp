#include "joj/renderer/font.h"

joj::Font::Font()
    : m_proportional(true), m_char_width(0),
    m_spacing{ 0 }, m_tex_width(0), m_tex_height(0)
{
}

joj::Font::Font(const char* filename)
    : m_proportional(true), m_char_width(0),
    m_spacing{ 0 }, m_tex_width(0), m_tex_height(0)
{
}

joj::Font::~Font()
{
}