#include "platform/window.h"

#include <iostream>

void window_print()
{
    std::cout << "Hello from Window!" << std::endl;
}

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
{
}

/*
 * FIXME: Hacky way.
 * Maybe put everything in a header file(?)
 */
#if JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
template class joj::Window<joj::WindowData>;
#endif // JPLATFORM_WINDOWS