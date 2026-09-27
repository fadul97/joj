#ifndef _JOJ_RHI_VK_PLATFORM_HPP
#define _JOJ_RHI_VK_PLATFORM_HPP

#include "joj/core/typedefs.h"

#include "joj/platform/display_server.hpp"

#ifdef JOJ_PLATFORM_LINUX
// XCB Includes
#include <xcb/xcb.h>

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>
#define JOJ_VK_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif

namespace joj::gfx::rhi::vk {

JOJ_API VkResult create_vulkan_surface(
    DisplayServer const* const display_server,
    VkInstance instance,
    VkAllocationCallbacks const* pAllocator,
    VkSurfaceKHR* pSurface);

} // namespace joj::gfx::rhi::vk

#endif // _JOJ_RHI_VK_PLATFORM_HPP
