#include "sandbox/gui_test.h"
#include "sandbox/app_test.h"
#include "joj/engine.h"
#include "joj/jmacros.h"

int main()
{
    joj::Engine engine;
    if JOJ_FAILED(engine.start())
        return -1;

    AppTest app;
    return engine.run(&app);
}
