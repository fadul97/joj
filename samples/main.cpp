#include "joj/platform/window.h"
#include <iostream>

int main()
{
    joj::Window window("My Window", 800, 600);
    std::cout << "Window title: " << window.get_title() << std::endl;
    window_print();
    return 0;
}
