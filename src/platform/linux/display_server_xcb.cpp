#include "joj/platform/display_server.hpp"
#include <xcb/xproto.h>

#ifdef JOJ_PLATFORM_LINUX

// STD Includes
#include <stdio.h>
#include <stdlib.h>

// OS Includes
#include <xcb/xcb.h>

// joj Includes
#include "joj/core/assert.hpp"

namespace joj {

ErrorCode display_server_create(DisplayServer* const display_server)
{
    JOJ_ASSERT(display_server);
    JOJ_ASSERT(display_server->data == nullptr);

    display_server->data = new DisplayServerData{};

    i32 screen_num = 0;
    display_server->data->connection = xcb_connect(nullptr, &screen_num);
    i32 ret = xcb_connection_has_error(display_server->data->connection);
    if (ret != 0)
    {
        printf("Failed to connect to server.\n");
        return ErrorCode::DISPLAY_SERVER_CONNECTION_FAILED;
    }

    xcb_setup_t const* setup = xcb_get_setup(display_server->data->connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    for (i32 i = 0; i < screen_num; ++i)
    {
        xcb_screen_next(&iter);
    }

    xcb_screen_t* screen = iter.data;

    u32 masks = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    u32 values[3] = { screen->white_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS, 0 };

    display_server->data->handle = xcb_generate_id(display_server->data->connection);
    if (display_server->data->handle < 0)
    {
        printf("Failed to generate Window ID.\n");
        return ErrorCode::DISPLAY_SERVER_HANDLE_CREATION;
    }

    [[maybe_unused]] xcb_void_cookie_t create_window_cookie = xcb_create_window(display_server->data->connection,
        XCB_COPY_FROM_PARENT,
        display_server->data->handle,
        screen->root,
        0, 0,
        800, 600,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        masks, values);

    xcb_map_window(display_server->data->connection, display_server->data->handle);
    xcb_flush(display_server->data->connection);

    return ErrorCode::OK;
}

void display_server_destroy(DisplayServer* const display_server)
{
    if (display_server->data->handle != 0)
    {
        xcb_unmap_window(display_server->data->connection, display_server->data->handle);
        xcb_flush(display_server->data->connection);

        xcb_destroy_window(display_server->data->connection, display_server->data->handle);
        display_server->data->handle = 0;
    }

    if (display_server->data->connection)
    {
        xcb_disconnect(display_server->data->connection);
        display_server->data->connection = nullptr;
    }

    delete display_server->data;
    display_server->data = nullptr;
}

b8 display_server_process_events(DisplayServer* const display_server)
{
    b8 ret = true;
    xcb_generic_event_t* event = nullptr;
    while ((event = xcb_poll_for_event(display_server->data->connection)))
    {
        switch (event->response_type & ~0x80)
        {
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t* key_press_event = (xcb_key_press_event_t*)event;
            // 9 = Escape key
            if (key_press_event->detail == 9)
            {
                ret = false;
            }
        }

        default:
            break;
        }

        if (event)
        {
            free(event);
            event = nullptr;
        }
    }

    return ret;
}

} // namespace joj

#endif // JOJ_PLATFORM_LINUX
