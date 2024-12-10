#ifndef _JOJ_WINDOW_H
#define _JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

JAPI void window_print();

namespace joj
{
    enum class WindowMode { Borderless, Fullscreen, Windowed };

    struct WindowRect
    {
        u32 left;
        u32 top;
        u32 right;
        u32 bottom;
    };

    template <typename T>
    class JAPI Window
    {
    public:
        Window();
        Window(const char* title, const u32 width, const u32 height);
        virtual ~Window();

        const T& get_data() const;

        const WindowRect& get_window_rect() const;
        const WindowRect& get_client_rect() const;

        u32 get_width() const;
        u32 get_height() const;

        const char* get_title() const;

        u32 get_xpos() const;
        u32 get_ypos() const;

    protected:
        T m_data;

        WindowRect m_window_rect;
        WindowRect m_client_rect;

        u32 m_width;
        u32 m_height;
        const char* m_title;

        u32 m_xpos;
        u32 m_ypos;
    };

    template <typename T>
    inline const T& Window<T>::get_data() const
    { return m_data; }

    template <typename T>
    inline const WindowRect& Window<T>::get_window_rect() const
    { return m_window_rect; }

    template <typename T>
    inline const WindowRect& Window<T>::get_client_rect() const
    { return m_client_rect; }

    template <typename T>
    inline u32 Window<T>::get_width() const
    { return m_width; }

    template <typename T>
    inline u32 Window<T>::get_height() const
    { return m_height; }

    template <typename T>
    inline const char* Window<T>::get_title() const
    { return m_title; }

    template <typename T>
    inline u32 Window<T>::get_xpos() const
    { return m_xpos; }

    template <typename T>
    inline u32 Window<T>::get_ypos() const
    { return m_ypos; }
}

#endif // _JOJ_WINDOW_H