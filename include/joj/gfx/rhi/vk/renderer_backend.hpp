#ifndef _JOJ_RHI_VK_RENDERER_BACKEND_HPP
#define _JOJ_RHI_VK_RENDERER_BACKEND_HPP

// Vulkan Includes
#include <vulkan/vulkan.h>

// 3rd Party Includes
#include <lft/fixed_vector.hpp>

// joj Includes
#include "joj/core/error/error_code.hpp"
#include "joj/core/typedefs.h"
#include "joj/gfx/rhi/typedefs.hpp"

namespace joj::gfx::rhi::vk {

class RendererBackend {
    JOJ_MAKE_DEFAULT_CTORS_AND_DTORS(RendererBackend);

public:
    ErrorCode initialize(DisplayServer const* const display_server) noexcept;
    void shutdown() noexcept;

private:
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
    VkRenderPass m_render_pass{ nullptr };
    lft::FixedVector<VkFramebuffer> m_framebuffers{};

    struct QueueFamilyIndices {
        u32 graphics_index{ JOJ_U32_MAX };
        u32 presentation_index{ JOJ_U32_MAX };
    };

    QueueFamilyIndices find_queue_families(VkPhysicalDevice physical_device);

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};

        lft::FixedVector<VkSurfaceFormatKHR> formats{};

        lft::FixedVector<VkPresentModeKHR> present_modes{};
    };

    SwapchainSupportDetails query_swapchain_support(VkPhysicalDevice physical_device);

    b8 is_physical_device_suitable(VkPhysicalDevice physical_device);

    void create_logical_device();

    void create_swapchain();

    void create_image_views();

    void create_render_pass();

    void create_framebuffers();
};

} // namespace joj::gfx::rhi::vk

#endif // _JOJ_RHI_VK_RENDERER_BACKEND_HPP
