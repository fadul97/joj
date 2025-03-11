/**
 * @file defines.h
 * @author Leonardo Fadul (lsffadul@gmail.com)
 * @brief This file contains type definitions that are used
 * throughout the engine and applications referencing it.
 * @version 0.1
 * @date 2025-03-11
 *
 * @copyright TODO: Copyright (c) 2025
 *
 * REFERENCES: Travis Vroman (https://travisvroman.com/) - Kohi Game Engine (https://github.com/travisvroman/kohi)
 *
 */

 #ifndef _JOJ_DEFINES_H
 #define _JOJ_DEFINES_H
 
 #include <cstdint>
 
 // Unsigned types
 
 /** @brief Unsigned 8-bit integer */
 typedef std::uint8_t u8;
 
 /** @brief Unsigned 16-bit integer */
 typedef std::uint16_t u16;
 
 /** @brief Unsigned 32-bit integer */
 typedef std::uint32_t u32;
 
 /** @brief Unsigned 64-bit integer */
 typedef std::uint64_t u64;
 
 // Signed types
 
 /** @brief Signed 8-bit integer */
 typedef std::int8_t i8;
 
 /** @brief Signed 16-bit integer */
 typedef std::int16_t i16;
 
 /** @brief Signed 32-bit integer */
 typedef std::int32_t i32;
 
 /** @brief Signed 64-bit integer */
 typedef std::int64_t i64;
 
 // Floating point types
 
 /** @brief 32-bit floating point number */
 typedef float f32;
 
 /** @brief 64-bit floating point number */
 typedef double f64;
 
 // Boolean types
 
 /** @brief 8-bit boolean type */
 typedef bool b8;
 
 /** @brief 32-bit boolean type */
 typedef int b32;
 
 /* JOJ_STATIC_ASSERT macro */
 #if defined(__GNUC__)
 #define JOJ_STATIC_ASSERT static_assert
 #elif defined(_MSC_VER)
 #define JOJ_STATIC_ASSERT static_assert
 #else
 #define JOJ_STATIC_ASSERT _Static_assert
 #endif
 
 // Ensure all types are of the correct size.
 
 /** @brief Assert u8 to be 1 byte.*/
 JOJ_STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
 
 /** @brief Assert u16 to be 2 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
 
 /** @brief Assert u32 to be 4 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
 
 /** @brief Assert u64 to be 8 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
 
 /** @brief Assert i8 to be 1 byte.*/
 JOJ_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
 
 /** @brief Assert i16 to be 2 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
 
 /** @brief Assert i32 to be 4 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
 
 /** @brief Assert i64 to be 8 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
 
 /** @brief Assert f32 to be 4 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
 
 /** @brief Assert f64 to be 8 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
 
 /** @brief Assert b8 to be 1 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(b8) == 1, "Expected b8 to be 1 byte.");
 
 /** @brief Assert b32 to be 4 bytes.*/
 JOJ_STATIC_ASSERT(sizeof(b32) == 4, "Expected f64 to be 4 bytes.");
 
 // Platform detection
 
 /* Windows OS */
 #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
 #define JOJ_PLATFORM_WINDOWS 1
 #elif defined(__linux__) || defined(__gnu__linux__)
 #define JOJ_PLATFORM_LINUX 1
 #else
 #error "Unknown platform!"
 #endif
 
 /* Compiler definition */
 #if defined(_MSC_VER)
 #define JOJ_COMPILER_MSVC 1
 #endif
 
 /* Extern macro */
 #define JOJ_EXTERN extern
 
 #ifdef JOJ_PLATFORM_WINDOWS
     #ifdef JOJ_BUILDING_DLL // Defined only in DLL compilation
         #define JOJ_API __declspec(dllexport)
         #define JOJ_GAME_API __declspec(dllimport)
     #elif defined(JOJ_USING_DLL) // Defines when using DLL
         #define JOJ_API __declspec(dllimport)
         #define JOJ_GAME_API __declspec(dllexport)
     #else
         #define JOJ_API  // Static builds
         #define JOJ_GAME_API // Static builds
     #endif
 #else
     #define JOJ_API
     #define JOJ_GAME_API
 #endif
 
 /* DEBUG macro */
 #if defined(DEBUG) || defined(_DEBUG) || JDEBUG_MODE
 #define JOJ_DEBUG_MODE 1
 #else
 #define JOJ_DEBUG_MODE 0
 #endif
 
 #endif // _JOJ_DEFINES_H
 