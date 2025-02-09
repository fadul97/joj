#ifndef _JOJ_DEVICE_CONTEXT_H
#define _JOJ_DEVICE_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

namespace joj
{
    template<typename T>
    class JAPI DeviceContext
    {
    public:
        DeviceContext();
        virtual ~DeviceContext() = 0;

        T& get() const;

    private:
        T m_context;
    };

    DeviceContext::DeviceContext() : m_context{} {}

    T& DeviceContext::get() const
    {
        return m_context;
    }
}

#endif // _JOJ_DEVICE_CONTEXT_H