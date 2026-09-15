#include "joj/joj.hpp"

// STD Includes
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// XCB Includes
#include <xcb/xcb.h>

namespace joj {

int main(Args const& args)
{
    if (args.argc > 1)
    {
        printf("Error: arg `%s` not expected.\n", args.argv[1]);
        return args.argc;
    }

    int screen_num = 0;
    xcb_connection_t* connection = xcb_connect(nullptr, &screen_num);
    int ret = xcb_connection_has_error(connection);
    if (ret != 0)
    {
        printf("Failed to connect to server.\n");
        return -1;
    }

    xcb_setup_t const* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    for (int i = 0; i < screen_num; ++i)
    {
        xcb_screen_next(&iter);
    }

    xcb_screen_t* screen = iter.data;

    printf("Information of screen %" PRIu32 ":\n", screen->root);
    printf("\tWidth: %" PRIu16 "\n", screen->width_in_pixels);
    printf("\tHeight: %" PRIu16 "\n", screen->height_in_pixels);
    printf("\tWhite pixel: %" PRIu32 "\n", screen->white_pixel);
    printf("\tBlack pixel: %" PRIu32 "\n", screen->black_pixel);

    printf("\tWidth - 1920: %" PRIu16 "\n", screen->width_in_pixels - 1920);

    unsigned int masks = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    unsigned int values[3] = { screen->white_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS, 0 };

    xcb_window_t window = xcb_generate_id(connection);
    if (window < 0)
    {
        printf("Failed to generate Window ID.\n");
        return -1;
    }

    [[maybe_unused]] xcb_void_cookie_t create_window_cookie = xcb_create_window(connection,
        XCB_COPY_FROM_PARENT,
        window,
        screen->root,
        0, 0,
        800, 600,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        masks, values);

    unsigned int gfx_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    unsigned int gfx_values[3] = { screen->black_pixel, 0, 0 };

    xcb_gcontext_t graphics_context = xcb_generate_id(connection);
    xcb_create_gc(connection, graphics_context, window, gfx_mask, gfx_values);

    xcb_map_window(connection, window);
    xcb_flush(connection);

    xcb_rectangle_t r = { 20, 20, 60, 60 };

    bool running = true;
    while (running)
    {
        xcb_generic_event_t* e = xcb_wait_for_event(connection);
        switch (e->response_type & ~0x80)
        {
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t* key_press_event = (xcb_key_press_event_t*)e;
            // 9 = Escape key
            if (key_press_event->detail == 9)
            {
                running = false;
            }
        }

        case XCB_EXPOSE:
            xcb_poly_fill_rectangle(connection, window, graphics_context, 1, &r);
            xcb_flush(connection);

        default:
            break;
        }

        if (e)
        {
            free(e);
        }
    }

    xcb_disconnect(connection);

    printf("Hello, JOJ!\n");
    return 0;
}

} // namespace joj
