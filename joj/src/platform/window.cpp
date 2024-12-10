#include "platform/window.h"

#include <iostream>

void window_print()
{
    std::cout << "Hello from Window!" << std::endl;
}

template <typename T>
joj::Window<T>::Window()
    : m_width{400}, m_height{400}, m_title{"Window"},
    m_xpos{0}, m_ypos{0}
{
}

template <typename T>
joj::Window<T>::Window(const char* title, const u32 width, const u32 height)
    : m_width{width}, m_height{height}, m_title{title},
    m_xpos{0}, m_ypos{0}
{}

template <typename T>
joj::Window<T>::~Window()
{
}
