#ifndef _LFT_STRING_HPP
#define _LFT_STRING_HPP

#include "assert.hpp"
#include "types.hpp"

namespace lft {

class String {
public:
    constexpr String() noexcept = default;

    constexpr String(char* const str) noexcept
    {
        char* s = nullptr;
        for (s = str; *s; ++s)
        {
            ;
        }

        m_length = static_cast<u32>(s - str);
        copy_str(str);
    }

    constexpr String& operator=(char* const str) noexcept
    {
        char* s = nullptr;
        for (s = str; *s; ++s)
        {
            ;
        }

        m_length = static_cast<u32>(s - str);
        copy_str(str);
        return *this;
    }

    constexpr ~String() noexcept
    {
        clear();
    }

    char const* const c_str() const noexcept
    {
        return m_data;
    }

    u32 length() const noexcept
    {
        return m_length;
    }

private:
    char* m_data{ nullptr };
    u32 m_length{ 0 };

    constexpr void clear() noexcept
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
            m_length = 0;
        }
    }

    constexpr void copy_str(char const* const str) noexcept
    {
        LFT_ASSERT(m_data == nullptr);

        m_data = new char[m_length + 1 /* Null terminator */];
        for (u32 i = 0; i < m_length; ++i)
        {
            m_data[i] = str[i];
        }

        m_data[m_length] = '\0';
    }
};

} // namespace lft

#endif // _LFT_STRING_HPP
