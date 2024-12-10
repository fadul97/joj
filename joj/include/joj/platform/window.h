#ifndef _JOJ_WINDOW_H
#define _JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

JAPI void window_print();

namespace joj
{
    class JAPI Window
    {
    public:
        Window();
        Window(const char* title, const u32 width, const u32 height);
        virtual ~Window();

        u32 get_width() const;
        u32 get_height() const;

        const char* get_title() const;

        u32 get_xpos() const;
        u32 get_ypos() const;

    protected:
        u32 m_width;
        u32 m_height;
        const char* m_title;

        u32 m_xpos;
        u32 m_ypos;
    };

    inline u32 Window::get_width() const
    { return m_width; }

    inline u32 Window::get_height() const
    { return m_height; }

    inline const char* Window::get_title() const
    { return m_title; }

    inline u32 Window::get_xpos() const
    { return m_xpos; }

    inline u32 Window::get_ypos() const
    { return m_ypos; }
}

#endif // _JOJ_WINDOW_H
