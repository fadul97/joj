#ifndef _LFT_TYPES_H
#define _LFT_TYPES_H

#include <cstdint>

namespace lft {

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using b8 = bool;
using b32 = std::uint32_t;

} // namespace lft

static_assert(sizeof(lft::u8) == 1);
static_assert(sizeof(lft::u16) == 2);
static_assert(sizeof(lft::u32) == 4);
static_assert(sizeof(lft::u64) == 8);

static_assert(sizeof(lft::i8) == 1);
static_assert(sizeof(lft::i16) == 2);
static_assert(sizeof(lft::i32) == 4);
static_assert(sizeof(lft::i64) == 8);

static_assert(sizeof(lft::f32) == 4);
static_assert(sizeof(lft::f64) == 8);

static_assert(sizeof(lft::b8) == 1);
static_assert(sizeof(lft::b32) == 4);

#define LFT_U32_MAX UINT32_MAX
#define LFT_U64_MAX UINT64_MAX

#endif // _LFT_TYPES_H
