#include "sandbox/gui_test.h"

f32 frametime = 0.0f;
b8 process_events();

int main()
{
    GUITest app;
    app.init();

    while (app.loop)
    {
        if (!process_events())
            app.loop = false;

        frametime = app.get_frametime();

        app.update(frametime);

        app.draw();
    }

    app.shutdown();

    return 0;
}

b8 process_events()
{
    MSG msg = { 0 };
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            return false;
    }

    return true;
}
