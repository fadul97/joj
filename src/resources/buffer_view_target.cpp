#include "joj/resources/buffer_view_target.h"

const char* joj::buffer_view_target_to_string(const BufferViewTarget target)
{
    switch (target)
    {
    case BufferViewTarget::ARRAY_BUFFER:          return "ARRAY_BUFFER";
    case BufferViewTarget::ELEMENT_ARRAY_BUFFER:  return "ELEMENT_ARRAY_BUFFER";
    case BufferViewTarget::UNKNOWN:               return "UNKNOWN";
    default:                                      return "UNKNOWN";
    }
}