#include "sandbox/gui_test.h"
#include "sandbox/app_test.h"
#include "sandbox/app2d_test.h"
#include "sandbox/app3d_test.h"
#include "joj/engine/engine.h"
#include "joj/core/jmacros.h"

int main()
{
    joj::Engine engine;
    if JOJ_FAILED(engine.start())
        return -1;

    GUITest app;
    return engine.run(&app);
}
