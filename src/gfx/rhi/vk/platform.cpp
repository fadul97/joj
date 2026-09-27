#include "joj/gfx/rhi/vk/platform.hpp"

namespace joj::gfx::rhi::vk {

VkResult create_vulkan_surface(
    DisplayServer const* const display_server,
    VkInstance instance,
    VkAllocationCallbacks const* pAllocator,
    VkSurfaceKHR* pSurface)
{
#ifdef JOJ_PLATFORM_LINUX
    VkXcbSurfaceCreateInfoKHR const surface_ci{
        // Structure ID
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        // Struct extension
        .pNext = nullptr,
        // For future use
        .flags = 0,
        // Connection to the X server
        .connection = display_server->data->connection,
        // Window handle
        .window = display_server->data->handle,
    };

    return vkCreateXcbSurfaceKHR(instance, &surface_ci, pAllocator, pSurface);
#endif
}

} // namespace joj::gfx::rhi::vk
