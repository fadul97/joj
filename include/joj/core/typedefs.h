#ifndef _JOJ_TYPEDEFS_H
#define _JOJ_TYPEDEFS_H

// ============================================================================
// For now, only build with C++ 20
//
#if __cplusplus < 202302L
#error "C++23 is required."
#endif

// ============================================================================
// Platform detection
//

/* Windows detection */
#if defined(_WIN32)
#define PLATFORM_NAME "windows"
#define JOJ_PLATFORM_WINDOWS
#elif defined(_WIN64)
#define PLATFORM_NAME "windows"
#define JOJ_PLATFORM_WINDOWS

/* Linux detection */
#elif defined(__linux__)
#define PLATFORM_NAME "linux"
#define JOJ_PLATFORM_LINUX

/* Other OSes will fail */
#else
#error "Operating System not supported."
#endif

// ============================================================================
// Dev or Release modes (in CMake or here?)
//
// #define JOJ_MODE_DEBUG

// ============================================================================
// Export macros
//
// TODO(leonardo): set visibility
#ifdef JOJ_PLATFORM_LINUX
#define JOJ_API
#else
#define JOJ_API
#endif

// ============================================================================
// Array len macro
//
#define JOJ_ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

// ============================================================================
// Default constructors and destructors
//
#define JOJ_MAKE_DEFAULT_CTORS_AND_DTORS(klass) \
public:                                         \
    klass() noexcept = default;                 \
    ~klass() noexcept = default;

// ============================================================================
// Custom copy operations on type (not sure if necessary)
//
#define JOJ_MAKE_CUSTOM_COPYABLE(klass) \
    klass(klass const&) noexcept;       \
    klass& operator=(klass const&) noexcept;

// ============================================================================
// Custom move operations on type (not sure if necessary)
//
#define JOJ_MAKE_CUSTOM_MOVABLE(klass) \
    klass(klass&&) noexcept;           \
    klass& operator=(klass&&) noexcept;

// ============================================================================
// Delete copy operations on type
//
#define JOJ_MAKE_NON_COPYABLE(klass) \
    klass(klass const&) = delete;    \
    klass& operator=(klass const&) = delete;

// ============================================================================
// Delete move operations on type
//
#define JOJ_MAKE_NON_MOVABLE(klass) \
    klass(klass&&) = delete;        \
    klass& operator=(klass&&) = delete;

#endif // _JOJ_TYPEDEFS_H
