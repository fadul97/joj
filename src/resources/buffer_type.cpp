#include "joj/resources/buffer_type.h"

const char* joj::buffer_type_to_string(const BufferType type)
{
    switch (type)
    {
    case BufferType::BYTE:          return "BYTE";
    case BufferType::POSITION:      return "POSITION";
    case BufferType::NORMAL:        return "NORMAL";
    case BufferType::INDEX:         return "INDEX";
    case BufferType::ANIMATION:     return "ANIMATION";
    case BufferType::TRANSLATION:   return "TRANSLATION";
    case BufferType::ROTATION:      return "ROTATION";
    case BufferType::SCALE:         return "SCALE";
    case BufferType::UNKNOWN:       return "UNKNOWN";
    default:                        return "UNKNOWN";
    }
}