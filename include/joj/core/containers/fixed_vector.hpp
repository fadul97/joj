#ifndef _JOJ_FIXED_VECTOR_HPP
#define _JOJ_FIXED_VECTOR_HPP

#include "joj/core/assert.hpp"

namespace joj {

template<typename T>
class FixedVector {
public:
    constexpr FixedVector() noexcept = default;
    constexpr explicit FixedVector(unsigned int const capacity) noexcept
        : m_capacity{ capacity }
    {
        alloc();
    }

    constexpr ~FixedVector() noexcept
    {
        clear();
    }

    constexpr T& operator[](unsigned int const index) const noexcept
    {
        JOJ_ASSERT(index < m_capacity);
        return m_data[index];
    }

    constexpr unsigned int capacity() const noexcept
    {
        return m_capacity;
    }

    constexpr void reserve(unsigned int const capacity)
    {
        m_capacity = capacity;
        alloc();
    }

private:
    T* m_data{ nullptr };
    unsigned int m_capacity{ 0 };
    bool m_allocated{ false }; // TODO(leonardo): maybe not needed.

    constexpr void clear() noexcept
    {
        if (m_data)
        {
            for (unsigned int i = 0; i < m_capacity; ++i)
            {
                m_data[i].~T();
            }

            delete[] m_data;
            m_data = nullptr;
            m_allocated = false;
        }
    }

    constexpr void alloc() noexcept
    {
        JOJ_ASSERT(m_data == nullptr);
        JOJ_ASSERT(!m_allocated);
        m_data = new T[m_capacity];
        JOJ_ASSERT(m_data);
        m_allocated = true;
    }
};

} // namespace joj

#endif // _JOJ_FIXED_VECTOR_HPP
