#include "joj/joj.hpp"

// STD Includes
#include <stdio.h>

// 3rd Party Includes
#include <lft/algorithm.hpp>

// joj Includes
#include "joj/core/assert.hpp"
#include "joj/core/typedefs.h"
#include "joj/core/types.h"
#include "joj/gfx/renderer.hpp"
#include "joj/platform/display_server.hpp"

namespace joj {

i32 main(MainArgs const& args)
{
    if (args.argv.capacity() > 1)
    {
        printf("Error: arg `%s` not expected.\n", args.argv[1].c_str());
        return args.argv.capacity();
    }

    // Debug
    for (u32 i = 0; i < args.argv.capacity(); ++i)
    {
        printf("Argv[%d]: `%s`\n", i, args.argv[i].c_str());
    }

    // ------------------------------------------------------------------------
    // Create Window
    // ------------------------------------------------------------------------

    DisplayServer display_server;
    JOJ_ASSERT_DEBUG(display_server.data == nullptr);

    if JOJ_FAILED (display_server_create(&display_server))
    {
        printf("[ERROR]: Failed to create DisplayServer.\n");
        return -1;
    }

    // ------------------------------------------------------------------------
    // Initialize Renderer
    // ------------------------------------------------------------------------

    gfx::Renderer renderer;
    if JOJ_FAILED (renderer.initialize(&display_server))
    {
        display_server_destroy(&display_server);
        return -1;
    }

    b8 running = true;
    while (running)
    {
        if (!display_server_process_events(&display_server))
        {
            running = false;
        }

        renderer.render();
    }

    renderer.shutdown();

    display_server_destroy(&display_server);

    printf("Hello, JOJ on %s!\n", PLATFORM_NAME);
    return 0;
}

} // namespace joj
