#include "joj/platform/window.h"
#include <iostream>
#include "joj/logger.h"

int main()
{
    joj::Window window{ "My Window Title", 800, 600 };
    std::cout << "Window title: " << window.get_title() << std::endl;
    JINFO("Working!");
    JDEBUG("Working!");
    JWARN("Working!");
    JERROR(joj::ErrorCode::FAILED, "Working!");
    JFATAL(joj::ErrorCode::FAILED, "Working!");
    window_print();
    return 0;
}
