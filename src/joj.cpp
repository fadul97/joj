#include "joj/joj.hpp"

// STD Includes
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// XCB Includes
#include <vulkan/vulkan_core.h>
#include <xcb/xcb.h>

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

// joj Includes
#include "joj/core/typedefs.h"
#include "joj/core/types.h"

#define VK_FAILED(result) ((result) != VK_SUCCESS)

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

    i32 screen_num = 0;
    xcb_connection_t* connection = xcb_connect(nullptr, &screen_num);
    i32 ret = xcb_connection_has_error(connection);
    if (ret != 0)
    {
        printf("Failed to connect to server.\n");
        return -1;
    }

    xcb_setup_t const* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    for (i32 i = 0; i < screen_num; ++i)
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

    u32 masks = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    u32 values[3] = { screen->white_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS, 0 };

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

    u32 gfx_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    u32 gfx_values[3] = { screen->black_pixel, 0, 0 };

    xcb_gcontext_t graphics_context = xcb_generate_id(connection);
    xcb_create_gc(connection, graphics_context, window, gfx_mask, gfx_values);

    xcb_map_window(connection, window);
    xcb_flush(connection);

    // ------------------------------------------------------------------------
    // Create Vulkan Instance
    // ------------------------------------------------------------------------

    // Describe application
    VkApplicationInfo const app_info{
        // ID of the struct
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        // Struct extension
        .pNext = nullptr,
        // Name of the application
        .pApplicationName = "Hello, Vulkan Instance",
        // Version of the application
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        // Name of the engine
        .pEngineName = "JOJ",
        // Version of the engine
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        // Vulkan API version requested
        .apiVersion = VK_API_VERSION_1_3,
    };

    char const* const extensions[]{
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XCB_SURFACE_EXTENSION_NAME
    };

    // Describe instance
    VkInstanceCreateInfo const instance_ci{
        // IF of the struct
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        // Struct extension
        .pNext = nullptr,
        // Behaviour of this instance
        .flags = 0,
        .pApplicationInfo = &app_info,
        // Number of global layers to enable
        .enabledLayerCount = 0,
        // Pointer to layer names
        .ppEnabledLayerNames = extensions,
        // Number of global extensions to enblae
        .enabledExtensionCount = 0,
        // Pointer to extension names
        .ppEnabledExtensionNames = nullptr,
    };

    VkAllocationCallbacks* m_allocator{ nullptr };

    VkInstance m_instance{ nullptr };
    VkResult result = vkCreateInstance(&instance_ci, m_allocator, &m_instance);
    if VK_FAILED (result)
    {
        printf("[ERROR]: Failed to create Vulkan Instance.\n");
        xcb_disconnect(connection);
        return -1;
    }

    xcb_rectangle_t r = { 20, 20, 60, 60 };

    b8 running = true;
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

    vkDestroyInstance(m_instance, m_allocator);

    xcb_disconnect(connection);

    printf("Hello, JOJ on %s!\n", PLATFORM_NAME);
    return 0;
}

} // namespace joj
