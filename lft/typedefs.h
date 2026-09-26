#ifndef _LFT_TYPEDEFS_H
#define _LFT_TYPEDEFS_H

// ============================================================================
// For now, only build with C++ 23
//
#if __cplusplus < 202302L
#error "C++23 is required."
#endif

// ============================================================================
// Platform detection
//

/* Windows detection */
#if defined(_WIN32)
#define LFT_PLATFORM_NAME "windows"
#define LFT_PLATFORM_WINDOWS
#elif defined(_WIN64)
#define LFT_PLATFORM_NAME "windows"
#define LFT_PLATFORM_WINDOWS

/* Linux detection */
#elif defined(__linux__)
#define LFT_PLATFORM_NAME "linux"
#define LFT_PLATFORM_LINUX

/* Other OSes will fail */
#else
#error "Platform not supported."
#endif

// ============================================================================
// Dev or Release modes (in CMake or here?)
//
// #define LFT_MODE_DEBUG

// ============================================================================
// Export macros
//
// TODO(leonardo): set visibility
#ifdef LFT_PLATFORM_LINUX
#define LFT_API
#else
#define LFT_API
#endif

// ============================================================================
// Array len macro
//
#define LFT_ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

// ============================================================================
// Custom copy operations on type (not sure if necessary)
//
#define LFT_MAKE_CUSTOM_COPYABLE(klass) \
    klass(klass const&) noexcept;       \
    klass& operator=(klass const&) noexcept;

// ============================================================================
// Custom move operations on type (not sure if necessary)
//
#define LFT_MAKE_CUSTOM_MOVABLE(klass) \
    klass(klass&&) noexcept;           \
    klass& operator=(klass&&) noexcept;

// ============================================================================
// Delete copy operations on type
//
#define LFT_MAKE_NON_COPYABLE(klass) \
    klass(klass const&) = delete;    \
    klass& operator=(klass const&) = delete;

// ============================================================================
// Delete move operations on type
//
#define LFT_MAKE_NON_MOVABLE(klass) \
    klass(klass&&) = delete;        \
    klass& operator=(klass&&) = delete;

#endif // _LFT_TYPEDEFS_H
