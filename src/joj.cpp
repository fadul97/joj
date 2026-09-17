#include "joj/joj.hpp"

// STD Includes
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// XCB Includes
#include <xcb/xcb.h>

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_xcb.h>

// joj Includes
#include "joj/core/error/error_code.hpp"
#include "joj/core/logging/logger.hpp"
#include "joj/core/typedefs.h"
#include "joj/core/types.h"

#define JOJ_VK_FAILED_AGAINST_SUCCESS(result) ((result) != VK_SUCCESS)
#define JOJ_VK_FAILED(result) ((result) != VK_SUCCESS && (result) != VK_INCOMPLETE)

namespace joj {

static b8 check_validation_layer_support()
{
    u32 layers_count{ 0 };
    VkResult result = vkEnumerateInstanceLayerProperties(&layers_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumerate Vulkan instance layer properties.\n");
        return false;
    }

    VkLayerProperties* layers = new VkLayerProperties[layers_count];
    result = vkEnumerateInstanceLayerProperties(&layers_count, layers);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumerate Vulkan instance layer properties.\n");
        delete[] layers;
        return false;
    }

    char const* const validation_layers[]{
#if JOJ_MODE_DEBUG
        "VK_LAYER_KHRONOS_validation"
#endif
    };

    for (u32 i = 0; i < layers_count; ++i)
    {
        printf("Validation layer[%d]: `%s`.\n", i, layers[i].layerName);
    }

    b8 layer_found{ false };
    for (u32 i = 0; i < layers_count; ++i)
    {
        if (strcmp(validation_layers[0], layers[i].layerName) == 0)
        {
            layer_found = true;
            break;
        }
    }

    delete[] layers;

    if (!layer_found)
    {
        return false;
    }

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    VkDebugUtilsMessengerCallbackDataEXT const* callback_data,
    void* user_data)
{
    switch (message_severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        printf("[VULKAN VERBOSE]: %s.\n", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        printf("[VULKAN WARNING]: %s.\n", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        printf("[VULKAN ERROR]: %s.\n", callback_data->pMessage);
        break;
    default:
        printf("[VULKAN UNKNOWN]: %s.\n", callback_data->pMessage);
        break;
    }

    // switch (message_type)
    // {
    // case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
    // case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
    // case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
    // default:
    //     break;
    // }

    return VK_FALSE;
}

static VkResult create_debug_utils_messenger_ext(
    VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const* create_info,
    VkAllocationCallbacks const* allocator, VkDebugUtilsMessengerEXT* debug_messenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    JOJ_ASSERT(func);

    return func(instance, create_info, allocator, debug_messenger);
}

static void destroy_debug_utils_messenger_ext(
    VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
    VkAllocationCallbacks const* allocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    JOJ_ASSERT(func);

    func(instance, debug_messenger, allocator);
}

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

    u32 extensions_count{ 0 };
    VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumate Vulkan instance extension properties.\n");
        xcb_disconnect(connection);
        return -1;
    }

    VkExtensionProperties* extension_properties = new VkExtensionProperties[extensions_count];
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, extension_properties);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumate Vulkan instance extension properties.\n");
        delete[] extension_properties;
        xcb_disconnect(connection);
        return -1;
    }

    for (u32 i = 0; i < extensions_count; ++i)
    {
        printf("Extension[%d]: `%s`.\n", i, extension_properties[i].extensionName);
    }

    delete[] extension_properties;

#if JOJ_MODE_DEBUG
    constexpr b8 ENABLE_VALIDATION_LAYERS{ true };
#else
    constexpr b8 ENABLE_VALIDATION_LAYERS{ false };
#endif

    char const* const validation_layers[]{
#if JOJ_MODE_DEBUG
        "VK_LAYER_KHRONOS_validation"
#endif
    };

    if (ENABLE_VALIDATION_LAYERS && !check_validation_layer_support())
    {
        printf("[ERROR]: Validation layers not supported.\n");
        xcb_disconnect(connection);
        return -1;
    }

    VkDebugUtilsMessengerCreateInfoEXT const debugger_ci{
        // ID of the struct
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        // Struct extension
        .pNext = nullptr,
        // Reserved for future use
        .flags = 0,
        // Which severy of events will call the debug callback function
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT // All diagnostics
            // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT                 // Informational message - can be helpful when debugging
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT // Expose a usage that may cause a bug
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,  // Condition violated
        // Which type of events will call the debug callback function
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT // Some general event ocurred
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT       // Something ocurred that may indicate invalid usage
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,     // Indicate something that might not be a non-optimal way of doing things
        // The application callback that should be called
        .pfnUserCallback = debug_callback,
        // Pointer to the any data the user wants to use in the callback
        .pUserData = nullptr,
    };

    // Describe instance
    VkInstanceCreateInfo const instance_ci{
        // ID of the struct
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        // Struct extension
        .pNext = &debugger_ci,
        // Behaviour of this instance
        .flags = 0,
        .pApplicationInfo = &app_info,
        // Number of global layers to enable
        .enabledLayerCount = 1,
        // Pointer to layer names
        .ppEnabledLayerNames = validation_layers,
        // Number of global extensions to enblae
        .enabledExtensionCount = 3,
        // Pointer to extension names
        .ppEnabledExtensionNames = extensions,
    };

    VkAllocationCallbacks* m_allocator{ nullptr };

    VkInstance m_instance{ nullptr };
    result = vkCreateInstance(&instance_ci, m_allocator, &m_instance);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to create Vulkan Instance.\n");
        xcb_disconnect(connection);
        return -1;
    }

    // ------------------------------------------------------------------------
    // Create Vulkan Debugger
    // ------------------------------------------------------------------------

#if JOJ_MODE_DEBUG
    VkDebugUtilsMessengerEXT m_debugger{ nullptr };

    result = create_debug_utils_messenger_ext(m_instance, &debugger_ci, m_allocator, &m_debugger);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to create Vulkan Debugger.\n");
        vkDestroyInstance(m_instance, m_allocator);
        xcb_disconnect(connection);
        return -1;
    }
#endif // JOJ_MODE_DEBUG

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

#if JOJ_MODE_DEBUG
    destroy_debug_utils_messenger_ext(m_instance, m_debugger, m_allocator);
#endif

    vkDestroyInstance(m_instance, m_allocator);

    xcb_disconnect(connection);

    for (u32 i = 0; i < static_cast<u32>(ErrorCode::MAX); ++i)
    {
        JOJ_LOG_TRACE("\t[%d]: `%s`.\n", i, error_code_to_cstr(static_cast<ErrorCode>(i)));
    }

    JOJ_TODO();

    printf("Hello, JOJ on %s!\n", PLATFORM_NAME);
    return 0;
}

} // namespace joj
