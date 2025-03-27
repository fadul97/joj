#include "joj/resources/component_type.h"

const char* joj::component_type_to_string(const ComponentType type)
{
    switch (type)
    {
    case ComponentType::BYTE:          return "BYTE";
    case ComponentType::UNSIGNED_BYTE: return "UNSIGNED_BYTE";
    case ComponentType::I16:           return "I16";
    case ComponentType::U16:           return "U16";
    case ComponentType::U32:           return "U32";
    case ComponentType::F32:           return "F32";
    case ComponentType::UNKNOWN:       return "UNKNOWN";
    default:                           return "UNKNOWN";
    }
}