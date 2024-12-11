#include <iostream>
#include "joj/logger.h"
#include "joj/platform/win32/window_win32.h"

int main()
{
    joj::Win32Window window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    if (window.create() != joj::ErrorCode::OK)
    {
        JFATAL(joj::ErrorCode::FAILED, "Failed to create Win32Window");
        return -1;
    }

    MSG msg = { 0 };
    bool loop = true;
    while (loop)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
                loop = false;
        }
    }

    return 0;
}
