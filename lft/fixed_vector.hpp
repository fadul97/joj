#ifndef _LFT_FIXED_VECTOR_HPP
#define _LFT_FIXED_VECTOR_HPP

#include "assert.hpp"
#include "types.hpp"

namespace lft {

template<typename T>
class FixedVector {
public:
    constexpr FixedVector() noexcept = default;
    constexpr explicit FixedVector(u32 const capacity) noexcept
        : m_capacity{ capacity }
    {
        alloc();
    }

    constexpr ~FixedVector() noexcept
    {
        clear();
    }

    constexpr T& operator[](u32 const index) const noexcept
    {
        LFT_ASSERT(index < m_capacity);
        return m_data[index];
    }
    constexpr void reserve(u32 const capacity)
    {
        m_capacity = capacity;
        alloc();
    }

    constexpr void clear() noexcept
    {
        if (m_data)
        {
            for (u32 i = 0; i < m_capacity; ++i)
            {
                m_data[i].~T();
            }

            delete[] m_data;
            m_data = nullptr;
            m_allocated = false;
            m_capacity = 0;
        }
    }

    constexpr b8 empty() const noexcept
    {
        return m_data == nullptr;
    }

    constexpr T* data() const noexcept
    {
        return m_data;
    }

    constexpr u32 capacity() const noexcept
    {
        return m_capacity;
    }

private:
    T* m_data{ nullptr };
    u32 m_capacity{ 0 };
    bool m_allocated{ false }; // TODO(leonardo): maybe not needed.

    constexpr void alloc() noexcept
    {
        LFT_ASSERT(m_data == nullptr);
        LFT_ASSERT(!m_allocated);
        m_data = new T[m_capacity];
        LFT_ASSERT(m_data);
        m_allocated = true;
    }
};

} // namespace lft

#endif // _LFT_FIXED_VECTOR_HPP
