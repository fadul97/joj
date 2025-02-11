// Testing dll building

#if 1

#include "joj/joj.h"
#include "joj/platform/window.h"
#include "joj/renderer/renderer.h"
#include "joj/renderer/d3d11/renderer_d3d11.h"
#include "joj/events/jevent.h"
#include "joj/math/jmath.h"
#include "joj/math/jvector3.h"
#include "joj/math/jvector4.h"
#include <iostream>

int main()
{
    joj::JVector3 v;
    joj::JVector4 v1(1.0f);
    joj::JVector4 v2(2.0);
    joj::JVector4 r = v1 + v2;
    std::cout << r.x << " " << r.y << " " << r.z << " " << r.w << "\n";

    joj_print();
    joj::window_print();
    joj::renderer_print();
    event_print();
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