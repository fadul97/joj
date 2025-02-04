#include <iostream>

#include "joj/platform/x11/window_x11.h"

int main()
{
    std::cout << "Hello, Linux!" << std::endl;

    joj::X11Window window = joj::X11Window("Test", 800, 600, joj::WindowMode::Windowed);
    if (window.create() != joj::ErrorCode::OK)
        std::cout << "Failed to create window. Got somewhere ;]" << std::endl;

    return 0;
}