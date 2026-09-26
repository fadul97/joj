#include "joj/joj.hpp"

// STD Includes
// #include <inttypes.h>
#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
// #include <unistd.h>

// XCB Includes
#include <xcb/xcb.h>

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_xcb.h>

// joj Includes
#include "joj/core/assert.hpp"
#include "joj/core/error/error_code.hpp"
#include "joj/core/lib.hpp"
#include "joj/core/logging/logger.hpp"
#include "joj/core/typedefs.h"
#include "joj/core/types.h"
#include "joj/gfx/renderer.hpp"
#include "joj/platform/display_server.hpp"

namespace joj {

template<class T>
constexpr T const& clamp(T const& v, T const& lo, T const& hi)
{
    return v < lo ? lo : ((v > hi) ? hi : v);
}

} // namespace joj

#define JOJ_VK_FAILED_AGAINST_SUCCESS(result) ((result) != VK_SUCCESS)
#define JOJ_VK_FAILED(result) ((result) != VK_SUCCESS && (result) != VK_INCOMPLETE)

namespace joj {

VkInstance m_instance{ nullptr };
VkAllocationCallbacks* m_allocator{ nullptr };
#if JOJ_MODE_DEBUG
VkDebugUtilsMessengerEXT m_debugger{ nullptr };
#endif
VkPhysicalDevice m_physical_device{ nullptr };
VkDevice m_device{ nullptr };
VkQueue m_graphics_queue{ nullptr };
VkSurfaceKHR m_surface{ nullptr };
VkQueue m_presentation_queue{ nullptr };
VkSwapchainKHR m_swapchain{ nullptr };
lft::FixedVector<VkImage> m_swapchain_images{};
VkFormat m_swapchain_image_format{ VK_FORMAT_MAX_ENUM };
VkExtent2D m_swapchain_extent{};
lft::FixedVector<VkImageView> m_swapchain_image_views{};

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

struct QueueFamilyIndices {
    u32 graphics_index{ JOJ_U32_MAX };
    u32 presentation_index{ JOJ_U32_MAX };
};

static QueueFamilyIndices find_queue_families(VkPhysicalDevice physical_device)
{
    QueueFamilyIndices indices;
    JOJ_ASSERT_DEBUG(indices.graphics_index == JOJ_U32_MAX);
    JOJ_ASSERT_DEBUG(indices.presentation_index == JOJ_U32_MAX);

    u32 queue_family_count{ 0 };
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

    VkQueueFamilyProperties* queue_families = new VkQueueFamilyProperties[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    i32 queues_index = 0;
    for (u32 i = 0; i < queue_family_count; ++i)
    {
        VkQueueFamilyProperties queue_family = queue_families[i];
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_index = queues_index;
        }

        VkBool32 present_support{ false };
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queues_index, m_surface, &present_support);
        if JOJ_VK_FAILED (result)
        {
            printf("[ERRO]: Failed to get Vulkan queue family presentation index.\n");
            return indices;
        }

        if (present_support)
        {
            indices.presentation_index = queues_index;
        }

        if (indices.graphics_index != JOJ_U32_MAX && indices.presentation_index != JOJ_U32_MAX)
        {
            break;
        }

        ++queues_index;
    }

    delete[] queue_families;
    queue_families = nullptr;

    return indices;
}

static b8 check_device_extension_support(VkPhysicalDevice physical_device)
{
    u32 extensions_count{ 0 };
    VkResult result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumerate device extensions.\n");
        return false;
    }

    VkExtensionProperties* available_extensions = new VkExtensionProperties[extensions_count];
    JOJ_ASSERT_DEBUG(available_extensions);
    result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_extensions);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumerate device extensions.\n");
        return false;
    }

    char const* const extensions[]{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    b8 extension_supported = false;
    for (u32 i = 0; i < extensions_count; ++i)
    {
        if (strcmp(extensions[0], available_extensions[i].extensionName) == 0)
        {
            printf("Extensions `%s` suppported.\n", extensions[0]);
            extension_supported = true;
        }
    }

    delete[] available_extensions;
    available_extensions = nullptr;

    return extension_supported;
}

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};

    lft::FixedVector<VkSurfaceFormatKHR> formats{};

    lft::FixedVector<VkPresentModeKHR> present_modes{};
};

SwapchainSupportDetails query_swapchain_support(VkPhysicalDevice physical_device)
{
    SwapchainSupportDetails details;
    JOJ_ASSERT_DEBUG(details.formats.data() == nullptr);
    JOJ_ASSERT_DEBUG(details.formats.capacity() == 0);
    JOJ_ASSERT_DEBUG(details.present_modes.data() == nullptr);
    JOJ_ASSERT_DEBUG(details.present_modes.capacity() == 0);

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &details.capabilities);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to Vulkan physical device surface capabilities.\n");
        return details;
    }

    u32 formats_count{ 0 };
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &formats_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to Vulkan physical device surface formats.\n");
        return details;
    }

    if (formats_count == 0)
    {
        printf("[ERROR]: NO Vulkan physical device surface format supported.\n");
        return details;
    }

    details.formats.reserve(formats_count);

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &formats_count, details.formats.data());
    if JOJ_VK_FAILED (result)
    {
        details.formats.clear();
        printf("[ERROR]: Failed to Vulkan physical device surface formats.\n");
        return details;
    }

    u32 present_modes_count{ 0 };
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_modes_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to Vulkan physical device surface present modes.\n");
        return details;
    }

    if (present_modes_count == 0)
    {
        printf("[ERROR]: NO Vulkan physical device surface present modes supported.\n");
        return details;
    }

    details.present_modes.reserve(present_modes_count);

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_modes_count, details.present_modes.data());
    if JOJ_VK_FAILED (result)
    {
        details.present_modes.clear();
        printf("[ERROR]: Failed to Vulkan physical device surface present modes.\n");
        return details;
    }

    return details;
}

static b8 is_physical_device_suitable(VkPhysicalDevice physical_device)
{
    QueueFamilyIndices indices = find_queue_families(physical_device);

    b8 const indices_complete = indices.graphics_index != JOJ_U32_MAX && indices.presentation_index != JOJ_U32_MAX;
    b8 const extensions_supported = check_device_extension_support(physical_device);

    b8 swapchain_adequate{ false };
    if (extensions_supported)
    {
        SwapchainSupportDetails swapchain_support = query_swapchain_support(physical_device);
        swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
    }

    return indices_complete && extensions_supported && swapchain_adequate;
}

static void create_logical_device()
{
    QueueFamilyIndices indices = find_queue_families(m_physical_device);

    u32 queue_count{ 1 };
    u32* unique_queue_families{ nullptr };
    if (indices.graphics_index == indices.presentation_index)
    {
        unique_queue_families = new u32[queue_count];
        unique_queue_families[0] = indices.graphics_index;
    }
    else
    {
        queue_count = 2;
        unique_queue_families = new u32[queue_count];
        unique_queue_families[0] = indices.graphics_index;
        unique_queue_families[1] = indices.presentation_index;
    }

    VkDeviceQueueCreateInfo* queue_cis = new VkDeviceQueueCreateInfo[queue_count];

    constexpr f32 queue_priority{ 1.0f };
    for (u32 i = 0; i < queue_count; ++i)
    {
        VkDeviceQueueCreateInfo const device_queue_ci{
            // Structure ID
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            // Struc extension
            .pNext = nullptr,
            // Behaviour of the queues
            .flags = 0,
            // Index of the queue in the device to create
            .queueFamilyIndex = unique_queue_families[i],
            // Number of queues to create
            .queueCount = 1,
            // Priorities of each queue
            .pQueuePriorities = &queue_priority,
        };

        queue_cis[i] = device_queue_ci;
    }

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(m_physical_device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(m_physical_device, &device_features);

    char const* const extensions[]{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 const extensions_count = JOJ_ARRAY_LEN(extensions);

    VkDeviceCreateInfo const device_ci{
        // Structure ID
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        // Struct extension
        .pNext = nullptr,
        // Reserved for future use
        .flags = 0,
        // Number of queues to create
        .queueCreateInfoCount = queue_count,
        // Pointer to queue create descriptions
        .pQueueCreateInfos = queue_cis,

        // enabledLayerCount is legacy and not used
        .enabledLayerCount = 0,
        // ppEnabledLayerNames is legacy and not used
        .ppEnabledLayerNames = nullptr,

        // Number of extensions to enable
        .enabledExtensionCount = extensions_count,
        // Pointer to array of extensions
        .ppEnabledExtensionNames = extensions,
        // All the features to be enabled
        .pEnabledFeatures = &device_features,
    };

    VkResult result = vkCreateDevice(m_physical_device, &device_ci, m_allocator, &m_device);
    if JOJ_VK_FAILED (result)
    {
        delete[] unique_queue_families;
        unique_queue_families = nullptr;
        delete[] queue_cis;
        queue_cis = nullptr;
        printf("[ERROR]: Failed to create Vulkan device.\n");
        abort();
    }

    vkGetDeviceQueue(m_device, indices.graphics_index, 0, &m_graphics_queue);
    vkGetDeviceQueue(m_device, indices.presentation_index, 0, &m_presentation_queue);

    delete[] unique_queue_families;
    unique_queue_families = nullptr;
    delete[] queue_cis;
    queue_cis = nullptr;
}

VkSurfaceFormatKHR select_swapchain_surface_format(lft::FixedVector<VkSurfaceFormatKHR> const& available_formats)
{
    for (u32 i = 0; i < available_formats.capacity(); ++i)
    {
        VkSurfaceFormatKHR const& format = available_formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            printf("[INFO]: Swapchain surface format: VK_FORMAT_B8G8R8A8_SRGB\n");
            printf("[INFO]: Swapchain surface color space: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR\n");
            return format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR select_swapchain_present_mode(lft::FixedVector<VkPresentModeKHR> const& available_present_modes)
{
    for (u32 i = 0; i < available_present_modes.capacity(); ++i)
    {
        if (available_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            printf("[INFO]: Swapchain present mode: VK_PRESENT_MODE_MAILBOX_KHR\n");
            return available_present_modes[i];
        }
    }

    printf("[INFO]: Swapchain present mode: VK_PRESENT_MODE_FIFO_KHR\n");
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D select_swapchain_extent(VkSurfaceCapabilitiesKHR const& capabilities)
{
    if (capabilities.currentExtent.width != JOJ_U32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        // TODO(leonardo): remove hardcoded swapchain extent size
        u32 const width{ 800 };
        u32 const height{ 600 };

        VkExtent2D const actual_extent{
            .width = clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            .height = clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };

        return actual_extent;
    }
}

static void create_swapchain()
{
    SwapchainSupportDetails swapchain_support = query_swapchain_support(m_physical_device);

    VkSurfaceFormatKHR const surface_format = select_swapchain_surface_format(swapchain_support.formats);
    VkPresentModeKHR const present_mode = select_swapchain_present_mode(swapchain_support.present_modes);
    VkExtent2D const extent = select_swapchain_extent(swapchain_support.capabilities);

    m_swapchain_image_format = surface_format.format;
    m_swapchain_extent = extent;

    u32 image_count = swapchain_support.capabilities.minImageCount + 1;
    if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount)
    {
        image_count = swapchain_support.capabilities.maxImageCount;
    }

    QueueFamilyIndices const indices = find_queue_families(m_physical_device);
    u32 queue_family_indices[]{ indices.graphics_index, indices.presentation_index };

    VkSharingMode image_sharing_mode{ VK_SHARING_MODE_MAX_ENUM };
    u32 queue_family_index_count{ 0 };
    u32* pqueue_family_indices{ nullptr };
    if (indices.graphics_index != indices.presentation_index)
    {
        image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
        queue_family_index_count = 2;
        pqueue_family_indices = queue_family_indices;
    }
    else
    {
        image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
        queue_family_index_count = 0;
        pqueue_family_indices = nullptr;
    }

    VkSwapchainCreateInfoKHR const swapchain_ci{
        // Structure ID
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        // Struct extension
        .pNext = nullptr,
        // Indicates parameters of the swapchain creation
        .flags = 0,
        .surface = m_surface,
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = image_sharing_mode,
        .queueFamilyIndexCount = queue_family_index_count,
        .pQueueFamilyIndices = pqueue_family_indices,
        .preTransform = swapchain_support.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = true,
        .oldSwapchain = nullptr,
    };

    VkResult result = vkCreateSwapchainKHR(m_device, &swapchain_ci, m_allocator, &m_swapchain);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to create Vulkan swapchain.\n");
        abort();
    }
}

static void create_image_views()
{
    m_swapchain_image_views.reserve(m_swapchain_images.capacity());

    for (u32 i = 0; i < m_swapchain_images.capacity(); ++i)
    {
        VkImageViewCreateInfo const image_view_ci{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = m_swapchain_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_swapchain_image_format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };

        VkResult result = vkCreateImageView(m_device, &image_view_ci, m_allocator, &m_swapchain_image_views[i]);
        if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
        {
            printf("[ERROR]: Failed to create Vulkan image view.\n");
            abort();
        }
    }
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

    DisplayServer display_server;
    JOJ_ASSERT_DEBUG(display_server.data == nullptr);

    if JOJ_FAILED (display_server_create(&display_server))
    {
        printf("[ERROR]: Failed to create DisplayServer.\n");
        return -1;
    }

    printf("\nDisplayServer created.\n");

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
        display_server_destroy(&display_server);
        return -1;
    }

    VkExtensionProperties* extension_properties = new VkExtensionProperties[extensions_count];
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, extension_properties);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumate Vulkan instance extension properties.\n");
        delete[] extension_properties;
        display_server_destroy(&display_server);
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
        display_server_destroy(&display_server);
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

    result = vkCreateInstance(&instance_ci, m_allocator, &m_instance);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to create Vulkan Instance.\n");
        display_server_destroy(&display_server);
        return -1;
    }

    // ------------------------------------------------------------------------
    // Create Vulkan Debugger
    // ------------------------------------------------------------------------

#if JOJ_MODE_DEBUG
    result = create_debug_utils_messenger_ext(m_instance, &debugger_ci, m_allocator, &m_debugger);
    if JOJ_VK_FAILED_AGAINST_SUCCESS (result)
    {
        printf("[ERROR]: Failed to create Vulkan Debugger.\n");
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }
#endif // JOJ_MODE_DEBUG

    // ------------------------------------------------------------------------
    // Create Vulkan Surface
    // ------------------------------------------------------------------------

    JOJ_ASSERT(display_server.data->connection != nullptr);
    VkXcbSurfaceCreateInfoKHR const surface_ci{
        // Structure ID
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        // Struct extension
        .pNext = nullptr,
        // For future use
        .flags = 0,
        // Connection to the X server
        .connection = display_server.data->connection,
        // Window handle
        .window = display_server.data->handle,
    };

    result = vkCreateXcbSurfaceKHR(m_instance, &surface_ci, m_allocator, &m_surface);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to create Vulkan surface for XCB.\n");
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }

    // ------------------------------------------------------------------------
    // Select Vulkan Physical Device
    // ------------------------------------------------------------------------

    u32 device_count{ 0 };
    result = vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumate Vulkan physical devices.\n");
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }

    if (device_count == 0)
    {
        printf("[ERROR]: No devies with Vulkan support.\n");
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }

    VkPhysicalDevice* physical_devices = new VkPhysicalDevice[device_count];
    result = vkEnumeratePhysicalDevices(m_instance, &device_count, physical_devices);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to enumate Vulkan physical devices.\n");
        delete[] physical_devices;
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }

    for (u32 i = 0; i < device_count; ++i)
    {
        VkPhysicalDevice physical_device = physical_devices[i];
        if (is_physical_device_suitable(physical_device))
        {
            m_physical_device = physical_device;
        }
    }
    delete[] physical_devices;
    physical_devices = nullptr;

    if (m_physical_device == nullptr)
    {
        printf("[ERROR]: Failed to find a suitable Vulkan physical device.\n");
        vkDestroyInstance(m_instance, m_allocator);
        display_server_destroy(&display_server);
        return -1;
    }

    // ------------------------------------------------------------------------
    // Create Vulkan Logical Device
    // ------------------------------------------------------------------------

    create_logical_device();

    // ------------------------------------------------------------------------
    // Create Vulkan Vulkan Swapchain
    // ------------------------------------------------------------------------

    create_swapchain();

    u32 swapchain_images_count{ 0 };
    result = vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchain_images_count, nullptr);
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to get Vulkan swapchain images.\n");
        return -1;
    }

    m_swapchain_images.reserve(swapchain_images_count);
    result = vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchain_images_count, m_swapchain_images.data());
    if JOJ_VK_FAILED (result)
    {
        printf("[ERROR]: Failed to get Vulkan swapchain images.\n");
        return -1;
    }

    create_image_views();

    b8 running = true;
    while (running)
    {
        if (!display_server_process_events(&display_server))
        {
            running = false;
        }
    }

    for (u32 i = 0; i < m_swapchain_image_views.capacity(); ++i)
    {
        vkDestroyImageView(m_device, m_swapchain_image_views[i], m_allocator);
    }
    m_swapchain_image_views.clear();

    m_swapchain_images.clear();

    if (m_swapchain)
    {
        vkDestroySwapchainKHR(m_device, m_swapchain, m_allocator);
        m_swapchain = nullptr;
    }

    if (m_device)
    {
        vkDestroyDevice(m_device, m_allocator);
        m_device = nullptr;
    }

    if (m_surface)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, m_allocator);
        m_surface = nullptr;
    }

#if JOJ_MODE_DEBUG
    if (m_debugger)
    {
        destroy_debug_utils_messenger_ext(m_instance, m_debugger, m_allocator);
        m_debugger = nullptr;
    }
#endif

    if (m_instance)
    {
        vkDestroyInstance(m_instance, m_allocator);
        m_instance = nullptr;
    }

    display_server_destroy(&display_server);

    for (u32 i = 0; i < static_cast<u32>(ErrorCode::MAX); ++i)
    {
        JOJ_LOG_TRACE("\t[%d]: `%s`.\n", i, error_code_to_cstr(static_cast<ErrorCode>(i)));
    }

    JOJ_TODO();

    gfx::Renderer renderer;
    renderer.initialize();
    renderer.shutdown();

    printf("Hello, JOJ on %s!\n", PLATFORM_NAME);
    return 0;
}

} // namespace joj
