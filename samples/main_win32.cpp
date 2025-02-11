// Testing dll building

#if 1

#include "joj/joj.h"
#include "joj/platform/window.h"

int main()
{
    joj_print();
    joj::window_print();
    return 0;
}

#else

#include "sandbox/gui_test.h"
#include "sandbox/app_test.h"
#include "sandbox/app2d_test.h"
#include "sandbox/app3d_test.h"
#include "joj/engine.h"
#include "joj/core/jmacros.h"

int main()
{
    joj::Engine engine;
    if JOJ_FAILED(engine.start())
        return -1;

    App3DTest app;
    return engine.run(&app);
}

#endif