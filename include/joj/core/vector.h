#ifndef _JOJ_VECTOR_H
#define _JOJ_VECTOR_H

#include "defines.h"

#include <vector>

namespace joj
{
    // TODO: Implement a custom vector

    typedef JOJ_API std::vector<i32> JVectorI32;  // Define vector with DLL interface

    template <typename T>
    class Vector
    {
    public:
        Vector();
        Vector(const size_t size);
        Vector(const size_t size, const T& default_value);
        Vector(const Vector<T>& other);
        Vector(Vector<T>&& other) noexcept;
        ~Vector();

        T& operator[](size_t index) { return m_data[index]; }
        Vector<T>& operator=(const Vector<T>& other)
        {
            if (this != &other)
            {
                delete[] m_data;

                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_data = new T[m_capacity];

                for (size_t i = 0; i < m_size; ++i)
                    m_data[i] = other.m_data[i];
            }

            return *this;
        }

        void push_back(const T& value);

        size_t get_size() const { return m_size; }
        size_t get_capacity() const { return m_capacity; }
        size_t get_memory_usage() const { return m_capacity * sizeof(T); }

        T* begin() { return m_data; }
        T* end() { return m_data + m_size; }

        const T* begin() const { return m_data; }
        const T* end() const { return m_data + m_size; }

        void clear();

        void pop_back();

    private:
        T* m_data;
        size_t m_size;
        size_t m_capacity;

        void reallocate(size_t new_capacity);

    };

    template <typename T>
    inline void Vector<T>::reallocate(size_t new_capacity)
    {
        // TODO: Return error?
        T* new_data = new(std::nothrow) T[new_capacity];
        if (new_data == nullptr)
        {
            m_size = 0;
            m_capacity = 0;
            return;
        }

        size_t elements_to_copy = (new_capacity < m_size) ? new_capacity : m_size;

        for (size_t i = 0; i < elements_to_copy; ++i)
            new_data[i] = std::move(m_data[i]);

        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;

        if (m_size > m_capacity)
            m_size = m_capacity;
    }

    template <typename T>
    Vector<T>::Vector()
        : m_data(nullptr), m_size(0), m_capacity(0) {}

    template <typename T>
    Vector<T>::Vector(const size_t size)
        : m_size(size), m_capacity(size)
    {
        m_data = new T[size]();
    }

    template <typename T>
    Vector<T>::Vector(const size_t size, const T& default_value)
        : m_size(size), m_capacity(size)
    {
        m_data = new T[size];
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = default_value;
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T>& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = new T[m_capacity];

        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    template <typename T>
    Vector<T>::Vector(Vector<T>&& other) noexcept
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        if (m_data)
            delete[] m_data;
    }

    template <typename T>
    inline void Vector<T>::push_back(const T& value)
    {
        if (m_size >= m_capacity)
            reallocate(m_capacity == 0 ? 1 : m_capacity * 2);

        m_data[m_size++] = value;
    }

    template <typename T>
    inline void Vector<T>::clear()
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }

        m_size = 0;
    }

    template <typename T>
    inline void Vector<T>::pop_back()
    {
        if (m_size > 0)
            --m_size;
    }
}

#endif // _JOJ_VECTOR_H