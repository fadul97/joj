#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <xcb/xcb.h>

int main(int argc, char** argv)
{
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

    xcb_window_t window = xcb_generate_id(connection);
    if (window < 0)
    {
        printf("Failed to generate Window ID.\n");
        return -1;
    }

    xcb_void_cookie_t create_window_cookie = xcb_create_window(connection,
        XCB_COPY_FROM_PARENT,
        window,
        screen->root,
        0, 0,
        150, 150,
        10,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        0, nullptr);

    xcb_map_window(connection, window);
    xcb_flush(connection);
    sleep(3);

    xcb_disconnect(connection);

    printf("Hello, World!\n");
    return 0;
}
