#include <iostream>
#include "joj/logger.h"
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"

int main()
{
    joj::Win32Window window{ "jojWindow", 800, 600, joj::WindowMode::Windowed };
    if (window.create() != joj::ErrorCode::OK)
    {
        JFATAL(joj::ErrorCode::FAILED, "Failed to create Win32Window");
        return -1;
    }

    u32 width = 0;
    u32 height = 0;

    window.get_window_size(width, height);
    JDEBUG("Window size: %dx%d", width, height);

    window.get_client_size(width, height);
    JDEBUG("Client size: %dx%d", width, height);

    joj::Win32Input input{};
    input.set_window(window.get_data());

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

        if (input.is_key_pressed('A'))
            JDEBUG("A pressed.");

        if (input.is_key_down(joj::KEY_SPACE))
            JDEBUG("Space down.");

        if (input.is_key_pressed(joj::KEY_ESCAPE))
            loop = false;
    }

    return 0;
}
