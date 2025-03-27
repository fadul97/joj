#ifndef _JOJ_COMPONENT_TYPE_H
#define _JOJ_COMPONENT_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class ComponentType
    {
        BYTE,
        UNSIGNED_BYTE,
        I16,
        U16,
        U32,
        F32,
        UNKNOWN
    };
    
    JOJ_API const char* component_type_to_string(const ComponentType type);
}

#endif // _JOJ_COMPONENT_TYPE_H