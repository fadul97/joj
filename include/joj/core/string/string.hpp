#ifndef _JOJ_STRING_HPP
#define _JOJ_STRING_HPP

#include "joj/core/assert.hpp"

namespace joj {

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

        m_length = static_cast<unsigned int>(s - str);
        copy_str(str);
    }

    constexpr String& operator=(char* const str) noexcept
    {
        char* s = nullptr;
        for (s = str; *s; ++s)
        {
            ;
        }

        m_length = static_cast<unsigned int>(s - str);
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

    unsigned int length() const noexcept
    {
        return m_length;
    }

private:
    char* m_data{ nullptr };
    unsigned int m_length{ 0 };

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
        JOJ_ASSERT(m_data == nullptr);

        m_data = new char[m_length + 1 /* Null terminator */];
        for (unsigned int i = 0; i < m_length; ++i)
        {
            m_data[i] = str[i];
        }

        m_data[m_length] = '\0';
    }
};

} // namespace joj

#endif // _JOJ_STRING_HPP
