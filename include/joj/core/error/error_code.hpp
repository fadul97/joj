#ifndef _JOJ_ERROR_CODE_HPP
#define _JOJ_ERROR_CODE_HPP

#include "joj/core/types.h"

#undef X
#define LIST_OF_ERROR_CODES                                                      \
    /** @brief Generic error code. */                                            \
    X(FAILED)                                                                    \
                                                                                 \
    /** @brief Used when assertion fails. */                                     \
    X(ASSERTION_FAILED)                                                          \
                                                                                 \
    /** @brief Used when DisplayServer connection fails. */                      \
    X(DISPLAY_SERVER_CONNECTION_FAILED)                                          \
                                                                                 \
    /** @brief Used when DisplayServer ID creation fails. */                     \
    X(DISPLAY_SERVER_HANDLE_CREATION)                                            \
                                                                                 \
    /* Vulkan Error Codes */                                                     \
                                                                                 \
    /** @brief Used for generic errors. */                                       \
    X(VULKAN_FAILED)                                                             \
                                                                                 \
    /** @brief Used whend backend fails to enumerate instance extensions. */     \
    X(VULKAN_ENUMERATE_EXTENSIONS)                                               \
                                                                                 \
    /** @brief Used whend backend fails to create a debug environment. */        \
    X(VULKAN_DEBUG_SUPPORT)                                                      \
                                                                                 \
    /** @brief Used whend backend fails to create a Vulkan instance. */          \
    X(VULKAN_INSTANCE_CREATION)                                                  \
                                                                                 \
    /** @brief Used whend backend fails to create a Vulkan debugger. */          \
    X(VULKAN_DEBUGGER_CREATION)                                                  \
                                                                                 \
    /** @brief Used whend backend fails to create a Vulkan surface. */           \
    X(VULKAN_SURFACE_CREATION)                                                   \
                                                                                 \
    /** @brief Used whend backend fails to enumerate physical devices. */        \
    X(VULKAN_ENUMERATE_PHYSICAL_DEVICES)                                         \
                                                                                 \
    /** @brief Used whend backend fails to find a device with Vulkan support. */ \
    X(VULKAN_SUPPORT)                                                            \
                                                                                 \
    /** @brief Used whend backend fails to find a suitable device. */            \
    X(VULKAN_PHYSICAL_DEVICE_INCOMPLETE)                                         \
                                                                                 \
    /** @brief Used whend backend fails to find get Swapchain images. */         \
    X(VULKAN_SWAPCHAIN_IMAGES_MISSING)                                           \
                                                                                 \
    /** @brief Used whend backend fails to create a Vulkan render pass. */       \
    X(VULKAN_RENDER_PASS_CREATION)                                               \
                                                                                 \
    /** @brief Used to signal maximum error value. */                            \
    X(MAX)

namespace joj {

enum class ErrorCode : u8 {
    /** @brief Used for successfull operations. */
    OK = 0,

#define X(err) err,
    LIST_OF_ERROR_CODES
#undef X
};

char const* error_code_to_cstr(ErrorCode const code);

} // namespace joj

#endif // _JOJ_ERROR_CODE_HPP
