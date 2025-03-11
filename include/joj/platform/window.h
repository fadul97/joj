#ifndef _JOJ_WINDOW_H
#define _JOJ_WINDOW_H

#include "core/defines.h"

#include "core/error_code.h"

namespace joj
{
    JOJ_API void window_print();
}

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

    struct WindowData;

    template <typename T>
    class Window
    {
    public:
        Window();
        Window(const char* title, const u32 width, const u32 height,
            const WindowMode mode);
        virtual ~Window();

        const T& get_data() const;

        const WindowRect& get_window_rect() const;
        const WindowRect& get_client_rect() const;

        u32 get_width() const;
        u32 get_height() const;

        virtual void get_window_size(u32& width, u32& height) = 0;
        virtual void get_client_size(u32& width, u32& height) = 0;

        const char* get_title() const;
        virtual void set_title(const char* title) = 0;

        u32 get_xpos() const;
        u32 get_ypos() const;

        virtual ErrorCode create() = 0;
        virtual void destroy() = 0;

    protected:
        T m_data;

        WindowRect m_window_rect;
        WindowRect m_client_rect;

        u32 m_width;
        u32 m_height;
        const char* m_title;

        WindowMode m_mode;

        u32 m_xpos;
        u32 m_ypos;
    };

    template <typename T>
    joj::Window<T>::Window()
        : m_width{0}, m_height{0}, m_title{""},
        m_xpos{0}, m_ypos{0}, m_mode{WindowMode::Windowed}
    {
        m_data = { 0 };

        m_window_rect = WindowRect
        {
            .left = 0,
            .top = 0,
            .right = 0,
            .bottom = 0,
        };

        m_client_rect = WindowRect
        {
            .left = 0,
            .top = 0,
            .right = 0,
            .bottom = 0,
        };
    }

    template <typename T>
    joj::Window<T>::Window(const char* title, const u32 width, const u32 height,
        const WindowMode mode)
        : m_width{width}, m_height{height}, m_title{title},
        m_xpos{0}, m_ypos{0}, m_mode{mode}
    {
        m_data = { 0 };

        m_window_rect = WindowRect
        {
            .left = 0,
            .top = 0,
            .right = width,
            .bottom = height,
        };

        m_client_rect = WindowRect
        {
            .left = 0,
            .top = 0,
            .right = width,
            .bottom = height,
        };
    }

    template <typename T>
    joj::Window<T>::~Window()
    {}

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